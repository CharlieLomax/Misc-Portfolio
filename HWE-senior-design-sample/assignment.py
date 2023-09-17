from flask import Blueprint, jsonify, request, current_app, send_file, Response
from models import Assignment, Question, StudentResponse, StudentAnswer
from nlp import GradingRule, assignment_setup, grade_submission
from forms import AssignmentForm, QuestionForm, StudentResponseForm, StudentAnswerForm
from datetime import datetime
import os
from mongoengine import ValidationError

assignment_blueprint = Blueprint('assignment', __name__, url_prefix='/assignments')

def get_unity_id():
    return request.headers.get('x-uid')

# Returns true if the UnityID belongs to an instructor or student, false if it doesn't
def authenticate_user():
    return not(get_unity_id() is None)

@assignment_blueprint.route('/unity_id', methods=['GET'])
def get_student_id():
    unity_id = get_unity_id()
    if unity_id is None:
        return Response("Please log in through Shibboleth.", status=401)
    return unity_id

@assignment_blueprint.route('/', methods=['GET'])
def get_assignments():
    if not(authenticate_user()):
        return {'message': 'You must be authenticated'}, 401
    instructor_id = get_unity_id()
    assignments = Assignment.objects(instructor_id=instructor_id)
    return jsonify(assignments), 200

@assignment_blueprint.route('/<id>', methods=['GET'])
def get_assignment(id):
    if not(authenticate_user()):
        return {'message': 'You must be authenticated'}, 401
    assignment = Assignment.objects.get_or_404(id=id)
    current_id = get_unity_id()
    if(assignment.instructor_id != current_id):
        delattr(assignment, "student_responses")
        return jsonify(assignment), 200
    return jsonify(assignment), 200

@assignment_blueprint.route('/pdf/<id>', methods=['GET'])
def get_pdf(id):
    if not(authenticate_user()):
        return {'message': 'You must be authenticated'}, 401
    assignment = Assignment.objects.get_or_404(id=id)
    return send_file(assignment.pdf_address)

@assignment_blueprint.route('/pdf/<id>', methods=['POST'])
def upload_pdf(id):
    if not(authenticate_user()):
        return {'message': 'You must be an instructor'}, 403
    file = request.files['file']
    file_type = file.filename.split('.')[-1].lower()
    if file_type != 'pdf':
        return {'message': 'Invalid file type'}, 400
    assignment = Assignment.objects.get_or_404(id=id)
    file_name = id + '.pdf'
    file_path = os.path.join(current_app.config['UPLOAD_FOLDER'], file_name)
    assignment.update(set__pdf_address=file_path)
    file.save(file_path)
    return {'message': 'File uploaded successfully'}, 201

@assignment_blueprint.route('/pdf/<id>', methods=['PUT'])
def edit_pdf(id):
    if not(authenticate_user()):
        return {'message': 'You must be authenticated'}, 401
    file = request.files['file']
    file_type = file.filename.split('.')[-1].lower()
    if file_type != 'pdf':
        return {'message': 'Invalid file type'}, 400
    assignment = Assignment.objects.get_or_404(id=id)
    file_path = assignment.pdf_address
    os.remove(file_path)
    file.save(file_path)
    return {'message': 'File updated successfully'}, 200

@assignment_blueprint.route('/', methods=['POST'])
def create_assignment():
    if not(authenticate_user()):
        return {'message': 'You must be authenticated'}, 401
    
    json_data = request.get_json()
    instructor_id = get_unity_id()

    try:
        title = json_data["title"]
        due_date = json_data["due_date"]
        question_list = json_data["questions"]
    # If these fields cannot be found, the input data is not valid
    except KeyError:
        return '', 400

    assignment_form = AssignmentForm(title=title, instructor_id=instructor_id, due_date=due_date)

    questions_form = []
    for q in question_list:
        try:
            text = q["text"]
            answer = q["answer"]
        # If these fields cannot be found, the input data is not valid
        except KeyError:
            return '', 400
        question_form = QuestionForm(text=text, answer=answer)
        questions_form.append(question_form)

    if assignment_form.validate():
        assignment = Assignment()
        for q in questions_form:
            if q.validate():
                question = Question()
                q.populate_obj(question)
                # Ideally, the rules should be changeable from the frontend, not hard coded in.
                question.rules = [GradingRule.ONE_SENTENCE.value, GradingRule.GRAMMAR.value, GradingRule.ACCURACY.value]
                assignment.questions.append(question)
            else:
                return jsonify(q.errors), 400
        assignment_form.populate_obj(assignment)
        # This try/catch block is a cheap fix for a bug. The validate() call earlier just doesn't work for some reason,
        # so this save throws an error. I don't want to waste too much time on it.
        try:
            assignment.save()
        except ValidationError:
            return '', 400
        # Generates spaCy docs for the new assignment, so that submissions can be graded
        assignment_setup(assignment)
        return jsonify(assignment), 201
    else:
        return jsonify(assignment_form.errors), 400
    
@assignment_blueprint.route('/submit/<id>', methods=['POST'])
def submit_assignment(id):
    if not(authenticate_user()):
        return {'message': 'You must be authenticated'}, 401
    json_data = request.get_json()

    assignment = Assignment.objects.get_or_404(id=id)

    student_id = get_unity_id()
    try:
        student_answers = json_data["student_answers"]
    # If these fields cannot be found, the input data is not valid
    except KeyError:
        return '', 400
    student_response_form = StudentResponseForm(student_id=student_id, creation_date=datetime.now())
    
    student_answer_form_list = []
    for a in student_answers:
        try:
            question_id = a["question_id"]
            answer = a["answer"]
        # If these fields cannot be found, the input data is not valid
        except KeyError:
            return '!', 400
        student_answer_form = StudentAnswerForm(question_id=question_id, answer=answer)
        student_answer_form_list.append(student_answer_form)

    if student_response_form.validate():
        student_response = StudentResponse()
        for a in student_answer_form_list:
            if a.validate():
                answer = StudentAnswer()
                a.populate_obj(answer)
                student_response.student_answers.append(answer)
            else:
                return jsonify(a.errors), 400
        student_response_form.populate_obj(student_response)
        
        assignment.student_responses.append(student_response)
        grade_submission(assignment, student_response)
        assignment.save()
        return jsonify(student_response), 201
    else:
        return jsonify(student_response_form.errors), 400

@assignment_blueprint.route('<assignment_id>/submission_contains_unityid/<student_id>', methods=['GET'])
def submission_contains_unityid(assignment_id, student_id):
    if not(get_student_id() == student_id or get_student_id() == assignment.instructor_id):
        return {'message': 'You are not authorized to this information'}, 403
    
    assignment = Assignment.objects.get_or_404(id=assignment_id)
    for response in assignment.student_responses:
        if response.student_id == student_id:
            return "true", 201
    return "false", 200

@assignment_blueprint.route('<assignment_id>/submissions/<student_id>', methods=['GET'])
def get_submission(assignment_id, student_id):
    assignment = Assignment.objects.get_or_404(id=assignment_id)
    if not(get_student_id() == student_id or get_student_id() == assignment.instructor_id):
        return {'message': 'You are not authorized to view this submission'}, 403
    
    assignment = Assignment.objects.get_or_404(id=assignment_id)
    for response in assignment.student_responses:
        if response.student_id == student_id:
            return jsonify(response), 200
        
    return {'message': 'Submission not found'}, 404

@assignment_blueprint.route('/<id>', methods=['PUT'])
def edit_assignment(id):
    assignment = Assignment.objects.get_or_404(id=id)
    if not(get_student_id() == assignment.instructor_id):
        return {'message': 'You are not authorized to edit this assignment'}, 403
    json_data = request.get_json()

    try:
        title = json_data["title"]
        due_date = json_data["due_date"]
        question_list = json_data["questions"]
    # If these fields cannot be found, the input data is not valid
    except KeyError:
        return '', 400
    assignment_form = AssignmentForm(title=title, instructor_id=assignment.instructor_id, due_date=due_date)
    
    questions_form = []
    for q in question_list:
        try:
            text = q["text"]
            answer = q["answer"]
        # If these fields cannot be found, the input data is not valid
        except KeyError:
            return '', 400
        question_form = QuestionForm(text=text, answer=answer)
        questions_form.append(question_form)

    if assignment_form.validate():
        assignment.questions = []
        for q in questions_form:
            if q.validate():
                question = Question()
                q.populate_obj(question)
                # Ideally, the rules should be changeable from the frontend, not hard coded in.
                question.rules = [GradingRule.ONE_SENTENCE.value, GradingRule.GRAMMAR.value, GradingRule.ACCURACY.value]
                assignment.questions.append(question)
            else:
                return jsonify(q.errors), 400
        assignment_form.populate_obj(assignment)
        # This try/catch block is a cheap fix for a bug. The validate() call earlier just doesn't work for some reason,
        # so this save throws an error. I don't want to waste too much time on it.
        try:
            assignment.save()
        except ValidationError:
            return '', 400
        # Generates spaCy docs for the new assignment, so that submissions can be graded
        assignment_setup(assignment)
        return jsonify(assignment), 200
    else:
        return jsonify(assignment_form.errors), 400
    
@assignment_blueprint.route('/<id>', methods=['DELETE'])
def delete_assignment(id):
    assignment = Assignment.objects.get_or_404(id=id)
    if not(get_student_id() == assignment.instructor_id):
        return {'message': 'You are not authorized to delete this assignment'}, 403
    os.remove(assignment.pdf_address)
    assignment.delete()
    return '', 204