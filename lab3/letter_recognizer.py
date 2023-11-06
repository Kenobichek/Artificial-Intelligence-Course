import numpy as np

class LetterRecognizer:
    def create_letter_matrix(self, letter):
        if letter == 'B':
            return np.array([
                [ 1,  1,  1,  1, -1],
                [ 1, -1, -1,  1, -1],
                [ 1,  1,  1,  1, -1],
                [ 1, -1, -1,  1, -1],
                [ 1,  1,  1,  1, -1]
            ])
        elif letter == 'M':
            return np.array([
                [ 1, -1, -1, -1,  1],
                [ 1,  1, -1,  1,  1],
                [ 1, -1,  1, -1,  1],
                [ 1, -1, -1, -1,  1],
                [ 1, -1, -1, -1,  1]
            ])
        elif letter == 'Я':
            return np.array([
                [-1,  1,  1,  1,  1],
                [-1,  1, -1, -1,  1],
                [-1,  1,  1,  1,  1],
                [-1, -1,  1, -1,  1],
                [-1,  1, -1, -1,  1]
            ])
        else:
            return None

    def recognize_letter(self, recognized_letter_index):
        if recognized_letter_index is not None:
            letters = ['B', 'M', 'Я']
            recognized_letter = letters[recognized_letter_index]
            print(f"The network recognized the letter: {recognized_letter}")
        else:
            print("The letter could not be recognized.")