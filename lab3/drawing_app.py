import tkinter as tk
import numpy as np

class DrawingApp:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Drawing Pixels")

        self.canvas_width, self.canvas_height = 500, 500
        self.pixel_size = 100

        self.canvas = tk.Canvas(self.root, width=self.canvas_width, height=self.canvas_height, bg='white')
        self.canvas.pack()

        self.pixels = np.zeros((5, 5))

        self.canvas.bind("<B1-Motion>", self.draw)

        convert_button = tk.Button(self.root, text="Get The Result", command=self.convert_and_display)
        convert_button.pack()

        self.root.mainloop()

    def draw(self, event):
        x, y = event.x, event.y
        x_idx, y_idx = int(x / self.pixel_size), int(y / self.pixel_size)
        self.canvas.create_rectangle(x_idx * self.pixel_size, y_idx * self.pixel_size,
                                (x_idx + 1) * self.pixel_size, (y_idx + 1) * self.pixel_size,
                                outline='black', fill='black')
        self.pixels[y_idx][x_idx] = 1

    def convert_and_display(self):
        converted_matrix = np.where(self.pixels == 1, 1, -1)
        print(converted_matrix)
        self.root.destroy()
