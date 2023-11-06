from hopfield_network import HopfieldNetwork
from drawing_app import DrawingApp
from letter_recognizer import LetterRecognizer

letter_recognizer = LetterRecognizer()
hopfield_network = HopfieldNetwork()

patterns = [
    letter_recognizer.create_letter_matrix('B'), 
    letter_recognizer.create_letter_matrix('M'), 
    letter_recognizer.create_letter_matrix('Я')
]

weights = hopfield_network.train_hopfield_network(patterns)

drawing_app = DrawingApp()

recognized_letter_index = hopfield_network.test_hopfield_network(weights, patterns, drawing_app.pixels)

letter_recognizer.recognize_letter(recognized_letter_index)
