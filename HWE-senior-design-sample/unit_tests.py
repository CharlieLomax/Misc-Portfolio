import datetime
import json
import unittest
from unittest.mock import patch, MagicMock
import flask_unittest
import spacy
import shutil, os

from app import create_app
from nlp import nlp, is_single_sentence, dict_setup, accuracy, has_correct_grammar
from models import Assignment, Question


class TestAssignmentAPI(flask_unittest.ClientTestCase):
    
    app = create_app()
    Assignment.drop_collection()

    # If there is an assignment with the given title, return it.
    # Assumes two assignments won't have the same title; if they do, it will only return the first one
    def get_assignment_by_name(self, name):
        assignments_with_name = Assignment.objects(title=name)
        if(len(assignments_with_name) > 0):
            return assignments_with_name[0]
        else:
            return None

    @patch('routes.assignment.get_unity_id', MagicMock(return_value='jjoof'))
    def setUp(self, client):
        # Check that only one assignment exists in the database
        response = client.get("/assignments/")

        self.assertEqual(response.status_code, 200)

        if (len(json.loads(response.data)) > 1):
            Assignment.drop_collection()

        # Get the assignment if it exists
        added_assignment = self.get_assignment_by_name("Title")

        # If the assignment doesn't exist, add it
        if(added_assignment == None):
            response = client.post('/assignments/', json={
            "title": "Title", 
            "due_date": "2023-12-1 23:59:59",
            "questions": [{"text": "question1", "answer": "answer1"}, {"text": "question2", "answer": "answer2"}]
            })
        
            self.assertEqual(response.status_code, 201)

            # Upload a PDF for the assignment
            added_assignment = self.get_assignment_by_name("Title")
            file_path = "backend/data/uploads/test.pdf"
            if(not(os.path.isfile(file_path))):
                shutil.copy("backend/test_files/test.pdf", file_path)
            added_assignment.update(set__pdf_address=file_path)

    @classmethod
    def tearDownClass(cls):
        os.remove("backend/data/uploads/test.pdf")

    @patch('routes.assignment.get_unity_id', MagicMock(return_value='jjoof'))
    def test_create_assignment(self, client):
        # Check that only one assignment exists in the database
        response = client.get("/assignments/")

        self.assertEqual(response.status_code, 200)

        self.assertEqual(len(json.loads(response.data)), 1)
        
        # Create an assignment
        response = client.post('/assignments/', json={
            "title": "Title1", 
            "due_date": "2023-11-1 23:59:59",
            "questions": [{"text": "question?", "answer": "42"}, {"text": "another question?", "answer": "UwU"}]
            })
        
        self.assertEqual(response.status_code, 201)

        # Check that a new assignment has been created
        response = client.get("/assignments/")

        self.assertEqual(response.status_code, 200)

        self.assertEqual(len(json.loads(response.data)), 2)

    @patch('routes.assignment.get_unity_id', MagicMock(return_value='jjoof'))
    def test_create_invalid_assignment(self, client):
        # Check the number of assignments that exists in the database
        response = client.get("/assignments/")

        self.assertEqual(response.status_code, 200)

        self.assertEqual(len(json.loads(response.data)), 1)

        # Attempt to create an assignment with an invalid date
        response = client.post('/assignments/', json={
            "title": "Title1", 
            "due_date": "2023-13-1 25:59:59", 
            "questions": []
            })
        
        self.assertEqual(response.status_code, 400)

        # Check that the invalid assignment was not added to the database
        response = client.get("/assignments/")

        self.assertEqual(response.status_code, 200)

        self.assertEqual(len(json.loads(response.data)), 1)

        # Attempt to create an assignment with an invalid question
        response = client.post('/assignments/', json={
            "title": "Title1", 
            "due_date": "2023-12-1 25:59:59", 
            "questions": [{"invalid_field": "garbage"}]
            })
        
        self.assertEqual(response.status_code, 400)

        # Check that the invalid assignment was not added to the database
        response = client.get("/assignments/")

        self.assertEqual(response.status_code, 200)

        self.assertEqual(len(json.loads(response.data)), 1)

        # Attempt to create an assignment without required fields
        response = client.post('/assignments/', json={
            "due_date": "2023-12-1 23:59:59", 
            })
        
        self.assertEqual(response.status_code, 400)

        # Check that the invalid assignment was not added to the database
        response = client.get("/assignments/")

        self.assertEqual(response.status_code, 200)

        self.assertEqual(len(json.loads(response.data)), 1)

    @patch('routes.assignment.get_unity_id', MagicMock(return_value='jjoof'))
    def test_get_assignment(self, client):
        # Get the assignment
        assignment_json = self.get_assignment_by_name("Title")

        # Get that assignment by id
        response = client.get("/assignments/" + str(assignment_json.id))

        self.assertEqual(response.status_code, 200)

        # Load the response data into a dict, and check that each field is the same as 
        # the assignment created ealier
        assignment = json.loads(response.data)
        self.assertEqual(assignment["title"], "Title")
        self.assertEqual(assignment["due_date"], {'$date': '2023-12-01T23:59:59Z'})
        self.assertEqual(assignment["questions"], [{"answer": "answer1", "rules": [1, 2, 3], "text": "question1"}, {"answer": "answer2", "rules": [1, 2, 3], "text": "question2"}])

    @patch('routes.assignment.get_unity_id', MagicMock(return_value='jjoof'))
    def test_delete_assignment(self, client):
        # Get the assignment
        assignment = self.get_assignment_by_name("Title")

        # Delete that assignment by id
        response = client.delete("/assignments/" + str(assignment.id))

        self.assertEqual(response.status_code, 204)

        # Check that the assignment was deleted and the database is empty
        response = client.get("/assignments/")

        self.assertEqual(response.status_code, 200)

        self.assertEqual(len(json.loads(response.data)), 0)

    @patch('routes.assignment.get_unity_id', MagicMock(return_value='jjoof'))
    def test_edit_assignment(self, client):
        # Get the assignment
        assignment = self.get_assignment_by_name("Title")

        # Check the number of assignments that exists in the database
        response = client.get("/assignments/")

        self.assertEqual(response.status_code, 200)

        self.assertEqual(len(json.loads(response.data)), 1)

        # Edit the assignment
        response = client.put('/assignments/' + str(assignment.id), json={
            "title": "Title2", 
            "due_date": "2023-12-2 23:59:59",
            "questions": [{"text": "question3", "answer": "answer3"}, {"text": "question4", "answer": "answer4"}]
            })
        
        self.assertEqual(response.status_code, 200)

        # Check that no new assignment has been added
        response = client.get("/assignments/")

        self.assertEqual(response.status_code, 200)

        self.assertEqual(len(json.loads(response.data)), 1)

        # Check the fields of the edited assignment
        edited_assignment = self.get_assignment_by_name("Title2")
        self.assertEqual(edited_assignment["title"], "Title2")
        self.assertEqual(edited_assignment["due_date"], datetime.datetime(2023, 12, 2, 23, 59, 59))
        self.assertEqual(edited_assignment["questions"], [Question(text="question3", answer="answer3", rules=[1, 2, 3]), Question(text="question4", answer="answer4", rules=[1, 2, 3])])

        # Check that the fields of the edited assignment are different from the original
        self.assertNotEqual(edited_assignment["title"], assignment["title"])
        self.assertNotEqual(edited_assignment["due_date"], assignment["due_date"])
        self.assertNotEqual(edited_assignment["questions"], assignment["questions"])

    @patch('routes.assignment.get_unity_id', MagicMock(return_value='jjoof'))
    def test_invalid_edit_assignment(self, client):
        # Get the assignment
        assignment = self.get_assignment_by_name("Title")

        # Attempt to edit the assignment with an invalid date
        response = client.put('/assignments/' + str(assignment.id), json={
            "title": "Title", 
            "due_date": "2023-13-1 25:59:59", 
            "questions": []
            })
        
        self.assertEqual(response.status_code, 400)

        # Check that the fields of the assignment are unedited
        edited_assignment = self.get_assignment_by_name("Title")
        self.assertEqual(edited_assignment["title"], assignment["title"])
        self.assertEqual(edited_assignment["due_date"], assignment["due_date"])
        self.assertEqual(edited_assignment["questions"], assignment["questions"])

        # Attempt to create an assignment with an invalid question
        response = client.put('/assignments/' + str(assignment.id), json={
            "title": "Title", 
            "due_date": "2023-12-1 25:59:59", 
            "questions": [{"invalid_field": "garbage"}]
            })
        
        self.assertEqual(response.status_code, 400)

        # Check that the fields of the assignment are unedited
        edited_assignment = self.get_assignment_by_name("Title")
        self.assertEqual(edited_assignment["title"], assignment["title"])
        self.assertEqual(edited_assignment["due_date"], assignment["due_date"])
        self.assertEqual(edited_assignment["questions"], assignment["questions"])

        # Attempt to create an assignment without required fields
        response = client.put('/assignments/' + str(assignment.id), json={
            "due_date": "2023-12-1 23:59:59", 
            })
        
        self.assertEqual(response.status_code, 400)

        # Check that the fields of the assignment are unedited
        edited_assignment = self.get_assignment_by_name("Title")
        self.assertEqual(edited_assignment["title"], assignment["title"])
        self.assertEqual(edited_assignment["due_date"], assignment["due_date"])
        self.assertEqual(edited_assignment["questions"], assignment["questions"])

    @patch('routes.assignment.get_unity_id', MagicMock(return_value='jjoof'))
    def test_submit_assignment(self, client):
        # Get the assignment
        assignment = self.get_assignment_by_name("Title")

        response = client.post('assignments/submit/' + str(assignment.id), json={
            "student_answers": [{"question_id": "question1", "answer": "answer1"}, {"question_id": "question2", "answer": "answer2"}]
            })
        
        self.assertEqual(response.status_code, 201)

        response = client.get('assignments/' + str(assignment.id) + '/submissions/jjoof')

        self.assertEqual(response.status_code, 200)
        self.assertEqual(len(response.json["student_answers"]), 2)
        self.assertEqual(response.json["highlights"], [])
        self.assertEqual(response.json["student_id"], "jjoof")

class TestNLP(unittest.TestCase):
    
    def setUp(self):
        Assignment.drop_collection()

    def test_generate_docs(self):
        # Since there are no assignments, the dict should be empty
        self.assertEqual(len(dict_setup()), 0)

        assignment = Assignment(title="Title", instructor_id="jjoof",
                                due_date=datetime.datetime(year=2024, month=5, day=5, hour=11, minute=59),
                                questions=[Question(text="Question1", answer="This is the answer.")])
        
        assignment.save()
        
        docs = dict_setup()
        # Now that an assignment has been created, the dict should have one entry, with the key being the assignment id.
        self.assertEqual(len(docs), 1)
        self.assertTrue(assignment.id in docs)
        # There should only be one entry in the inner dict, since there is only one question
        self.assertEqual(len(docs[assignment.id]), 1)
        self.assertTrue("Question1" in docs[assignment.id])
        # This object should be a Doc object generated by spaCy.
        # Since the specific values are generated by spaCy, not by our code, we won't test them.
        self.assertTrue(type(docs[assignment.id]["Question1"]) == spacy.tokens.doc.Doc)
    
    def test_is_single_sentence(self):
        self.assertTrue(is_single_sentence(nlp("This is a single sentence.")))
        self.assertFalse(is_single_sentence(nlp("This is a sentence. This is a second sentence.")))

    def test_correct_grammar(self):
        self.assertTrue(has_correct_grammar(nlp("This is a sentence."), "This is a sentence."))
        self.assertFalse(has_correct_grammar(nlp("This is a sentence"), "This is a sentence"))
        self.assertFalse(has_correct_grammar(nlp("this is a sentence."), "this is a sentence."))

    def test_is_accurate(self):
        doc = nlp("The dog chased a car.")
        doc1 = nlp("The dog pursued a motorcycle.")
        doc2 = nlp("Dogs are not real.")

        acc1 = accuracy(doc, doc1, 100, 0)
        acc2 = accuracy(doc, doc2, 100, 0)

        # Since the model might change, we can't test exact numbers, but they should always be between 0 and 100
        self.assertTrue(100 >= acc1)
        self.assertTrue(100 >= acc2)
        self.assertTrue(0 <= acc1)
        self.assertTrue(0 <= acc2)
        
        # Also, doc1 should be more similar than doc2 to the gold standard
        self.assertTrue(acc2 < acc1)
        


if __name__ == '__main__':
    unittest.main()