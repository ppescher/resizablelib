/*!
@mainpage

@section whatsthis What is it?

  This class library is an attempt to make the development of
resizable windows a little easier for the MFC developer and much more
pleasant for the user. A resizable window in this context is a window,
not necessarily top-level, that once resized either by the user or the
developer, is able to automatically resize and reposition its child
controls appropriately.

@section howtouse Using the library

  To use the class library perform the following steps:
<ul>
<li>Unzip the library source and put the ResizableLib directory in a place
of your choice. I suggest to use the same directory where you create your
Projects (e.g. "C:\MyProjects") or your common path for 3rd-party libraries 
<li>Go to the FileView pane and right click on the root element, or choose
the "Project" menu, then select "Insert Project into Workspace..."
<li>Navigate to the place where you put the library, select the library
Project file and check "Dependency of" specifying which Project will make
use of the library 
<li>Re-activate your Project in the FileView and open the Settings dialog
(also from the "Project" menu) 
<li>Make sure your Project is selected in the tree view and that you have
selected "All Configurations" in the combo box, then click on the "C/C++" tab 
<li>In the "Category" combo box choose "Preprocessor" and add the library
path to the "Additional include directories" edit box 
</ul>
  This procedure is valid for VC++ 6.0 only and it is the suggested way of
employment for new users of the library. Users of VC++ 7.0 and later versions
can easily find corresponding steps for the new IDE.
  More experienced users may want to follow their own method to use the
library in their projects. This includes, but it is not limited to, the
following methods:
<ul>
<li>Add the required library source files to the project, provided that
directions in the library precompiled header are followed in the project
precompiled header
<li>Build the library independently and link to the proper output file
manually, optionally adding the library source files directory to either
the project settings or the IDE include path
</ul>
 */

/*!
@page license Artistic License
You may obtain a copy of this document from:
http://www.opensource.org/licenses/artistic-license.html
@verbinclude Artistic-License.txt
 */

/*!
@defgroup CoreComponents Core Components

This module contains classes and structures that implement core
functionalities of this library, such as layout management, size
constraints, persisting windows layout and user interface elements
used in resizable windows.
 */
