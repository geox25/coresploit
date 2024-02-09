# Dependencies Manifest

Modified 2/9/2024

---

PXL should work fine on any system that has OpenGL installed where the compiler can find it (specified in cmakelists.txt).
Most likely you won't need ``xorg`` or ``xorg-dev``, but if it is broken and errors mention them you should install them.
If the compiler can't find opengl, try installing the ``opengl3`` package on Ubuntu or through a different method if you are
on a different Operating System.

Ubuntu Package Dependencies:
- xorg                (imgui)
- xorg-dev            (imgui)
- opengl3             (imgui)
