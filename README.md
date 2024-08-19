### GPU Rendering with OpenGL

Project for 42 Ecole.

This project loads models from .obj files with mtl settings, sends data to the GPU, and renders using shaders. You can rotate models along the x and y axes, move them along the x, y, and z axes, and change their size.

You can also rotate textures, select the active object (which will light up), and toggle simple lights on and off.

According to the task, I couldn't use any external libraries to parse the model, work with matrices, or load textures.

So, I created my own parser, a small analog of glm functions, and some simple code to load textures as bmp files.

Additionally, I used the stb_image library to load jpg textures just for fun stuff, as it is not part of the main subject.

**Usage:**

To render a model, add the path to the .obj file as the second argument. Example: `./scop pathTo.obj`

**Controls:**

- `WASD` to move the object along the xy axes
- `Q` and `E` to move the object along the z axis
- `UP`, `DOWN`, `LEFT`, `RIGHT` to rotate the object
- `T` to toggle between texture color modes
- `F` for lines only
- `U` for fill mode
- `-+` to scale the object (use left shift for +)
- To turn on the light, type `lumos`. Can you guess how to turn it off?



https://github.com/user-attachments/assets/1f5eb481-d9ae-4722-9848-ff17ba61ba32



https://github.com/user-attachments/assets/2eaa4e84-3e31-4559-92c5-12c5954f811e



https://github.com/user-attachments/assets/ae029fd5-aac2-44e3-b48f-cccbe043067f



https://github.com/user-attachments/assets/af4cd434-65f6-4cc9-8ced-97479fb1ac06

