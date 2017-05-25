# Speccer

**Description**

Speccer is a program used for visualising any kind of tree data. Its power comes from the fact that it's entirely up to the users to decide what kind of data the tree holds and how it's organised and also change the look and feel (from now on referred to as *style*) of the tree with the click of a button. Data is shared between styles, so if a style has a text property called *name*, all data under that property will be transferred to properties with the same name of all other styles.

Another major feature is that users can define their own styles. Infact, this is one of the basic premises of the program. Although any and all styles are meant to be compatible, as of know some care is needed to name properties consistently, so that properties are carried over between different styles (e.g. info instead of Info, information of Information). This issue will be looked to be resolved in the future.

Other than not being completely stable and optimised for speed it represents a minimal viable product.

**Table of Contents**

* Installation
* Usage
* Current issues
* Contributing
* Internal structure
* Credits
* License

**Installation**

No instalation is required, just copy the appropriate for your OS executable along with all its accompanying folders and files. Or just download the respective .zip and unarchive it.

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

**Contributing**

*To be expanded.*

**Internal structure**

*To be expanded.*

**Credits**

*To be expanded.*

**License**

GNU GPLv3 License
