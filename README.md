# Speccer

**Description**

Speccer is a program used for visualising any kind of tree data. Its power comes from the fact that it's entirely up to the users to decide what kind of data the tree holds and how it's organised and also change the look and feel (from now on referred to as *style*) of the tree with the click of a button. Data is shared between styles, so if a style has a text property called *name*, all data under that property will be transferred to properties with the same name of all other styles.

Another major feature is that users can define their own styles. Infact, this is one of the basic premises of the program. Although any and all styles are meant to be compatible, as of know some care is needed to name properties consistently, so that properties are carried over between different styles (e.g. info instead of Info, information of Information). This issue will be looked to be resolved in the future.

Other than not being completely stable and optimised for speed it represents a minimal viable product.

**Table of Contents**

* Installation
* Usage
* Current issues
* To Do
* Contributing
* Internal structure
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

*To be expanded.*

**Current issues**

* Instability - the program is known to crash upon window closure, window reopening (for the second time), moving the view after window reopening and very rarely in other cases. The cause of almost all, if not all, of these crashes is known to stem from wxWidgets and the (mis)use of some of its functions. However, as the crashes occur upon window closure (or later) and not immediately after the faulty code, it's very difficult to trace the ultimate cause.<br/>Although these crashes normally do not affect operation, it's still advised to save regularly and create backup files, as the program hasn't yet been tested extensively.

* Speed - close to no optimisation has been done yet for working with many nodes. Currently it begins to lag only after merely few dozen of branches have been placed. One of the highest priority goals for the next release is to make it work with hundreds of nodes without any noticeable lag.

* Incomplete support for composite shapes - currently formulas for composite shapes don't always compute correctly in the tree editor, and the usage of composite shapes within composite shapes hasn't been tested at all.

* Other currently unimplemented functionality:
  * Different node type per branch
  * Consecutive transformations for children branches
  * Calculation and usage of transformed coordinates
  * Acquiring screenshots of trees

**To Do**

A list of things planned for future versions. Note that none of this has any ETA.

  v1.0.0.1
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

*To be expanded.*

**Credits**

See /license/Special cases.txt for a list of 3rd party code used.

**License**

GNU GPLv3 License - Located in /license/GNU General Public License version 3.txt
