# input bindings

an Input Binding is a set of Input Mappings

## Input Mappings

An input mapping is the context to trigger the action and the action triggered.

### Input Action

- Input Action
    - **Binary Input Action** <br>
    (should be configurable whether or not active/inacitve is called simultaneaously to activating/deactivating)
        - Activatating / Pressed / Entered
        - Active / Held / Inside
        - Deactivating / Released / Exited
        - Inactive / Unpressed / Outside
    - DimensionalInputAction
        - InputAction1D\<numeric>
        - InputAction2D\<numeric>
        - InputAction3D\<numeric>
    
### Input Mapping

#### Binary Action Mappings

- button/key press
- region (1, 2, or 3 axes) (axis/axes value(s) changed and fall within the region represented)
    - Range (1d, defined by min and max value)
    - Area* (There are many ways to define an area, this should allow the intersection function to be passed in a la comparator)
    - Volume* (Same as with area, should allow the intersection function to be passed in)

\* Will require a bunch of intersection functions, 
(this seems like something that should be reusable with collision, maybe provided by directx math lib)

- 2D
    - Circle
    - Square
    - Triangle
    - Pie Slice (Min/Max Angles)
    - Arbitrary 2D Geometry?
- 3D  (flight sticks techincally do this, but i can't image how one would use this in a game)
    - Sphere
    - Box
    - Cylinder
    - Cone

#### Dimensional Input Mappings

- axis (1, 2, or 3 axes)
    - value changed (allow change threshold?)
- angle (2 axes) (probably neeed a naming scheme for +- xy, xz, yz angles, research me, may already exist)
    - Angle Changed
- heading (3 axes)
    - Heading Changed


### Complex Mapping

Must be composed of binary action mappings?

- complex input (Multiple inputs required)(Should include axis input, and allow ranges to be specified for the axes)
    - Combination
        - Activated (may include an interval in which all inputs need to be achieved, eg, all pressed within one second)
        - Held (should be configurable as to how many buttons have to stop being held in order to deactive)
        - Deactivated
    - Sequence
        - Activated (Buttons pressed and/or released in a particular order, may specify timing on a per button)
        - Partial Activation (part of the sequence has been inputted, should trigger after each item in the sequence)
        - Canceled (some of the sequence was inputted but the entire sequence failed due to timing.)

### TODO

X button mapping