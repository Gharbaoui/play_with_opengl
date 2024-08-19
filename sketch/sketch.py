from manim import *

class Sketch(ThreeDScene):
    def construct(self):
        self.set_camera_orientation(phi=PI/4, theta=-PI/2 + PI/4, zoom=.75)

        axes = ThreeDAxes(
            
        )

        axes_labels = axes.get_axis_labels()
        self.add(axes, axes_labels)

