from enum import Enum
import spacy

from models import Assignment

class GradingRule(Enum):
    ONE_SENTENCE = 1
    GRAMMAR = 2
    ACCURACY = 3

# Load the NLP model. We're using the large model, since it has a large number of word vectors built in
nlp = spacy.load('en_core_web_lg')

# The dict that contains docs for the gold standard answers.
# Access a gold standard doc using '_gold_standards_dict[assignment.id][question.text]'
_gold_standards_dict = dict()

_question_rules_dict = dict()

# Generate a nested dict to store the spaCy docs for the gold standard responses.
# The outer dict is indexed by assignment ID, and contains dicts storing the question data for that assignment.
# The inner dict is indexed by question text, and contains the spaCy doc of the gold standard answer.
def dict_setup():
    global _gold_standards_dict
    global _question_rules_dict
    _question_rules_dict= dict()
    _gold_standards_dict = dict()
    
    for assignment in Assignment.objects():
       assignment_setup(assignment)

    return _gold_standards_dict

def assignment_setup(assignment):
    global _gold_standards_dict
    global _question_rules_dict
    _gold_standards_dict[assignment.id] = generate_assignment_docs(assignment)
    _question_rules_dict[assignment.id] = generate_rule_dict(assignment)

def assignment_cleanup(assignment):
    global _gold_standards_dict
    global _question_rules_dict
    del _gold_standards_dict[assignment.id]
    del _question_rules_dict[assignment.id]

# Generates spaCy docs for a single assignment
def generate_assignment_docs(assignment):
    answer_dict = dict()
    for question in assignment.questions:
        answer_dict[question.text] = nlp(question.answer)
    return answer_dict

def generate_rule_dict(assignment):
    answer_dict = dict()
    for question in assignment.questions:
        answer_dict[question.text] = question.rules
    return answer_dict

# Get the grade on a student submission
def grade_submission(assignment, submission):
    for student_answer in submission.student_answers:
        student_answer.feedback = grade_question(
            _gold_standards_dict[assignment.id][student_answer.question_id], 
            student_answer.answer, _question_rules_dict[assignment.id][student_answer.question_id])
    

# Get the grade of an individual question
def grade_question(gold_standard, answer, rules):
    doc = nlp(answer)
    
    feedback = dict()

    if(GradingRule.ONE_SENTENCE.value in rules):
        feedback[GradingRule.ONE_SENTENCE.name] = is_single_sentence(doc)
    if(GradingRule.GRAMMAR.value in rules):
        feedback[GradingRule.GRAMMAR.name] = has_correct_grammar(doc, answer)
    if(GradingRule.ACCURACY.value in rules):
        feedback[GradingRule.ACCURACY.name] = accuracy(gold_standard, doc, .8, .35)
    
    return feedback

# Determines whether the response is only a single sentence
def is_single_sentence(doc):
    # Gets the number of sentences- can't be done with a len() function since doc.sents is a generator
    sentences_in_doc = sum(1 for _ in doc.sents)
    # Return true if there is only one sentence in the response, false otherwise
    return sentences_in_doc == 1

# Determines whether the sentence is grammatically correct, spelled correctly, and only a single sentence
def has_correct_grammar(doc, answer):
    if(not(answer[0].isupper() and answer[len(answer) - 1] == ".")):
        return False
    return True

# Determines whether the answer is accurate to the gold standard answer.
# 'full_credit_dist' is the similarity value at which the student will recieve full credit;
# 'no_credit_dist' is the similarity value at which the student will no longer recieve even partial credit.
# Returns a number between 0 and 100, representing credit the student gets for accuracy.
def accuracy(gold_standard, doc, full_credit_dist, no_credit_dist):
    # Calculate the word vector similarity between the student answer doc and the gold standard doc
    dist = gold_standard.similarity(doc)
    if(dist >= full_credit_dist):
        # Full credit
        return 100
    elif(dist < no_credit_dist):
        # No credit
        return 0
    else:
        # Partial credit. Returns the percent of the way between "no credit" and "full credit".
        return  100 * (dist - no_credit_dist) / (full_credit_dist - no_credit_dist)

# This code is useful for getting a feel for the return values of the similarity function's return values.
# I left it in in case you need to tweak any of the similarity stuff.
#if __name__ == '__main__':
#    while(True):
#        str1 = input("Input the first string: ")
#        str2 = input("Input the second string: ")
#        print("Similarity: " + str(nlp(str1).similarity(nlp(str2))))