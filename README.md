# Speccer

**Description**

Speccer is a program used for visualising any kind of tree data. Its power comes from the fact that it's entirely up to the users to decide what kind of data the tree holds and how it's organised and also change the look and feel (from now on referred to as *style*) of the tree with the click of a button. Data is shared between styles, so if a style has a text property called *name*, all data under that property will be transferred to properties with the same name of all other styles.

Another major feature is that users can define their own styles. Infact, this is one of the basic premises of the program. Although any and all styles are meant to be compatible, as of know some care is needed to name properties consistently, so that properties are carried over between different styles (e.g. info instead of Info, information of Information). This issue will be looked to be resolved in the future.

Other than not being completely stable and optimised for speed it represents a minimal viable product.

**Table of Contents**

* Installation
* Usage
* Known issues
* To Do
* Contributing
* Internal structure
* Formula syntax
* Text fields syntax
* Make other fields for user input
* Credits
* License

**Installation**

You can download the program from *executables/\<your OS\>/*

No instalation is required, just copy the appropriate for your OS executable along with all its accompanying folders and files. Or just download the respective .zip and unarchive it.

**For Windows you may also need to have *mingwm10.dll* in the program's folder.**

**Usage**

The program opens in a fairly small main window, giving access to the two main modes of operation - the tree editor and the style editor.

Most of the work occurs in the tree editor, where the actual trees are being built.

The style editor, as the name implies, is used to create user defined styles(technically the prebuilt styles are no different than the user defined styles). The style editor also has a submode of operation, used to make new composite shapes.

The interface should be intuitive. It would be helpful to read the section *internal structure* to read on terms and concepts such as *composite*, *formula* and *node*. Also read the section *formula syntax* to read on how to write formulas in style edit mode.

**Known issues**

* Instability - the program is known to crash upon window closure, window reopening (for the second time), moving the view after window reopening and very rarely in other cases. The cause of almost all, if not all, of these crashes is known to stem from wxWidgets and the (mis)use of some of its functions. However, as the crashes occur upon window closure (or later) and not immediately after the faulty code, it's very difficult to trace the ultimate cause.<br/>Although these crashes normally do not affect operation, it's still advised to save regularly and create backup files, as the program hasn't yet been tested extensively.

* Speed - close to no optimisation has been done yet for working with many nodes. Currently it begins to lag only after merely few dozen of branches have been placed. One of the highest priority goals for the next release is to make it work with hundreds of nodes without any noticeable lag.
  * One minor source of lag, but not a dealbreaker, is parsing all of the formulas. This will ultimately have to be solved, but it isn't a priority for now.
  * A major source of lag, though not the only one, is *FT_Load_Char*. Caching would solve the problem.
  * Besides that there seems to be a slightly bigger, still unidentified, source of lag.
  
* Images - currently images are only able to be imported if they're in the same folder as the saved file - **the document has to be saved first!** Also, paths to images are stored in the saved file, instead of images per se, so if the image files are renamed, moved or deleted, they won't be loaded anymore in the saved file.

* Text fields and other input controls are sometimes created minimized or become minimized. Maximizing and then unmaximizing (or the other way around) the window solves the issue.

* Incomplete support for composite shapes - currently formulas for composite shapes don't always compute correctly in the tree editor, and the usage of composite shapes within composite shapes hasn't been tested at all.

* Some other minor bugs and issues mentioned in the to do list.

* Other currently unimplemented functionality:
  * Different node type per branch
  * Consecutive transformations for children branches
  * Calculation and usage of transformed coordinates
  * Acquiring screenshots of trees

**To Do**

A list of things planned for future versions. Note that none of this has any ETA.

  v1.0.0.X - won't necessarily be published, only if the need arises. It's basically for two things:
*   Critical bug fixes
*   Features taking one or two lines of code

  v1.0.1
*    Optimise for speed - ability to work comfortably with hundreds of branches
* Scale text up & down when zooming
* More basic shapes:
  * Fix drawing of composite formulas when used as drawables
  * Arrow - as a composite
*    Fix segfault on reopening the style editor (& moving the plotter)
*    Make it possible for each branch to pick its own node out of the available nodes in the style
*    NURBS curve as a basic shape
*    Completely get rid of wxNewId() - it's deprecated
*    Get rid of wxToolBarBase::AddTool() - it's deprecated
*    When saving and loading convert underscore to space and viceversa
* Bool values to control visibility of drawables
  * And to be able to be interpreted as 1's and 0's in formulas
*    Display style names instead of file paths in the tree editor - with underscores converted to spaces
*    Consecutive transformations between nodes - for spiraling and and zooming in trees
*    Bold & italic for text
*    Custom icons for placing drawables
*    Detect when changes are made to the currently open file
*    Fix a bug where after loading any applied transformation matrices are not displayed in the menu as such
* Fix an issue where font name in font picker in style editor is no longer displayed after text deselection
  * Fix minor bug causing label shift and occasional item deselection when shuffling placed drawables
*    Red formula input fields upon incorrect syntax
*    Toolbar in style editor
*    Fix line drawing for angles less than or equal to 90 degrees
*    Vertex and shape hints (which one is selected) when in style edit mode
*    Fix short textboxes when first time displaying
*    Fix widths in list controls
* Acquire screenshots
  * Of the whole tree with margins
  * Of a selected area

  v1.1
* Global node to set background, timescale & other constant & one time drawn stuff
  * Special syntax for drawing of repeating elements
*    Check for matrix & node loops when loading
*    Calculate transformed coordinates
*    Allow to use composite shapes within composite shapes
*    Menu shortcuts
*    Remake the interface with floatable subwindows & toolbars + everything to be in one window
*    Group properties and global variables in subgroups
*    Optional popup windows for entering formulas & text
*    Make collapsable groups of nodes for clade groupings
*    Optimize for speed and small file sizes for work with 1000s of nodes
*    Undo & redo functionality ?
*    Copy & paste

**Contributing**

Contributing is most appreciated when done for things planned for future versions, but other relevant and useful contributions are also welcome. Even if you don't know how to program, you can still help:
* The program could use better icons.
* Icons will also be used in the style editor, for the different shape types and one for composites.
* The program itself needs an icon, which will double serve as logo.
* More default styles are also welcome, as long as they look well, handle edge cases and can be used for multiple purposes (are not made for one specific thing only).
* When the relevant functionality is more robust - more default composite shapes.

**Internal structure**

Some sort of object relationship diagram should be published in the future. Instead of it, for now there's this description of some of the classes:

**Datapoint** represents the atomic data pieces stored in the program. They can be texts (textdata), numbers (numberdata), images (imagedata) among others.

**Formuladata** is a special kind of *datapoint* that serves mostly for determining the size of location of various elements. As its name implies, it contians formulas instead of bare numbers.

**Formulaparser** serves to parse the formulas contained by various instances of *formuladata*, among some minor auxiliary functionalities.

**Shape** represents the actual things to be drawn by the program. Shapes are e.g. circles, rectangles, texts, images among others. There are four control shapes (root, anchor, bounding box, branching point) to aid the drawing of trees and nodes and one shape for storing transformations (transformation matrix). Unlike datapoints, there's one class for all shapes.

**Composite** contains a collection of shapes to quickly build repeeating visual elements, such as arrows, textboxes and others.

**Drawable** is an abstract class that represents the concept of anything that is able to be drawn by the program. It is inherited by *shape* and *composite*.

**Dataholder** represents a node. Just like a composite, it has a collection of drawables to draw. It's a type of composite, but it doesn't inherit from it, partly because this was realised too late in production, and partly because it shouldn't inherit from *drawable*.

**Datamodel** represents a style. It contains a collection of nodes (dataholders). The default nodes is called *Leaf* and there should always be one per style. In future, branches will be able to be different nodes and there'll be one control node called *global* to define the overall look and feel of the style.

**DrawingAPI and UIAPI** are abstract classes representing the drawing plot and the user interface, respectively. The reason for their existance is so that the core program is independant from the current way of implementing the drawing and UI and that the change of both is as simple as creating new classes inheriting from those. **No classes, except for colorconv(see below) and those inheriting from DrawingAPI and UIAPI, should make any assumptions as to what technologies are used for drawing or user interface.**

**Oglplotter** is the current realisation of *DrawingAPI*. It uses OpenGL 1.4 to draw the *drawables*.

**Styleframe and treeframe** are the current realisations of *UIAPI* for the two program modes. They use WxWidgets 3.0.

**Tree** is used to store the tree data in tree edit mode, **branch** for branch data. **Colorconv** is an auxiliary class to store colours in a drawing technology-independant way and convert colours between various types of storage. Finally, **serialisable** is an abstract class used for the saving/loading functionality. Any program data that saves and loads inherits from it.

**Formula syntax**

* They mostly take the shape of standard mathematical expressions - numbers, the four operators + - / \*, brackets - with some additions.
* ^ _ are two special operators to take respectively the maximum and minimum of two numbers. They have the same order of precedence as multiplication and division.
* There are three built in constants: pi, tau (double of pi) and phi (the golden ratio).
* There are also some per bracnh constants to help with the layout of the tree:
  * sc, sw, sh - sibling count, total width and total height. Note that the width and height of a branch is only really the width and height of its bounding box. Its position is the position of its root, which coincides with the branching point of its parent, or with (0, 0) if its the root branch.
  * dc, dw, dh - descendant count, total width and total height.
  * udc, udw, udh - ultimate descendant count, total width and total height. Ultimate as in only of those (in)direct descendants that don't have descendants themselves. If the node doesn't have any descendants, udc is 1 and udw and udh are its own width and height.
  * n is represents the value of whichth the node is in its parent's descendant hierarchy. E.g. if a node has five descendants, the first one has n of 1, the second one of 2, the third one of 3, etc.
  * pc, pw, ph - preceding sibling count, total width and total height - only of those siblings with n lesser than the node's own n.
  * In general, the formula for a node's vertical position is *ph + pc\*10 + (udh + udc\*10 - sh - sc\*10)/2*, where 10 is the distance between sibling nodes in pixels. If the tree is organised top-down isntead, the formula for a node's horizontal position is *pw + pc\*10 + (udw + udc\*10 - sw - sc\*10)/2*.
* There are the following built in functions: sin(), cos(), tan(), cotan(), asin(), acos(), atan(), acotan(), round(), sqrt() and abs().
* The x and y coordinates and width and height of *drawables* can be taken by writing the respective drawable's name between angle brackets, followed by one of the letters x, y, w and h, e.g. \<rectangle\>x, \<text\>w, \<second circle\>h.
  * These are only absolute coordinates and sizes, i.e. before any transformations are applied. In the future, transformed coordinates will also be able to be used in formulas.
  * Beware of cyclic dependancies. For example, if a rectangle's width depends on \<circle\>x and a circle's x position depends on \<rectangle\>w, neither would be calculated correctly, as they both rely on the other one being calculated first. The order of drawables in the drawable list in style edit mode is the order in which drawable formulas are paresd.
* User input values can be created by writing the name of the desired value between square brackets, e.g. [age], [extinction]. These input values should be numbers only.
* If an invalid or uncomputable expression is met, it evaluates to 0 and evaluation halts. Note that only it returns 0 and not the whole formula. So, for example, *100 + pi/0 + 10* returns 100.

**Text fields syntax**

Only in a style edit mode, to let the user have a new text field in tree edit mode, surround that field in square brackets. E.g. *Name: [name]* will let the user input the value of *name* while in tree edit mode, and that will substitute *[name]*. More than one text field is possible per text. If a text doesn't have any text fields, the text will be static for all branches.

**Make other fields for user input**

Again while in style edit mode, right clicking over a image field or a color field label will let you input a surface name for that respective data. Later in tree edit mode, the user will be able to input unique image or color datas per branch.

**Credits**

See /license/Special cases.txt for a list of 3rd party code used.

**License**

GNU GPLv3 License - Located in /license/GNU General Public License version 3.txt
