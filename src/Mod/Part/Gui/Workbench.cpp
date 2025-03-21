/***************************************************************************
 *   Copyright (c) 2005 Werner Mayer <wmayer[at]users.sourceforge.net>     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"

#include "Workbench.h"
#include <Base/Interpreter.h>
#include <Gui/MenuManager.h>
#include <Gui/ToolBarManager.h>

using namespace PartGui;

#if 0 // needed for Qt's lupdate utility
    qApp->translate("Workbench", "&Part");
    qApp->translate("Workbench", "&Simple");
    qApp->translate("Workbench", "&Parametric");
    qApp->translate("Workbench", "Solids");
    qApp->translate("Workbench", "Part tools");
    qApp->translate("Workbench", "Boolean");
    qApp->translate("Workbench", "Primitives");
    qApp->translate("Workbench", "Join");
    qApp->translate("Workbench", "Split");
    qApp->translate("Workbench", "Compound");
    qApp->translate("Workbench", "Create a copy");
#endif

/// @namespace PartGui @class Workbench
TYPESYSTEM_SOURCE(PartGui::Workbench, Gui::StdWorkbench)

Workbench::Workbench() {
    /** If we are to have Sketcher_NewSketch as command in toolbar and menu,
     then we must assure SketcherGui has already been loaded.
     By putting this in a try/except block we avoid creating a dependency
     on sketcher workbench as the import will silently fail if sketcher wb is not built.
     Note that BUILD_SKETCHER is a cmake-gui option.
     **/

    const char* code =
            "try:\n"
            "    import SketcherGui\n"
            "    success = 'True'\n"
            "except ImportError:\n"
            "    success = 'False'";

    const std::string result = Base::Interpreter().runStringWithKey(code, "success", "False");
    hasSketcher = (result == "True");

}

Workbench::~Workbench() = default;

Gui::MenuItem* Workbench::setupMenuBar() const
{
    Gui::MenuItem* root = StdWorkbench::setupMenuBar();
    Gui::MenuItem* item = root->findItem("&Windows");

    Gui::MenuItem* prim = new Gui::MenuItem;
    prim->setCommand("Primitives");
    *prim << "Part_Box"
          << "Part_Cylinder"
          << "Part_Sphere"
          << "Part_Cone"
          << "Part_Torus"
          << "Separator"
          << "Part_Tube";

    Gui::MenuItem* copy = new Gui::MenuItem;
    copy->setCommand("Create a copy");
    *copy << "Part_SimpleCopy"
          << "Part_TransformedCopy"
          << "Part_ElementCopy"
          << "Part_RefineShape";

    Gui::MenuItem* bop = new Gui::MenuItem;
    bop->setCommand("Boolean");
    *bop << "Part_Boolean"
         << "Part_Cut"
         << "Part_Fuse"
         << "Part_Common";

    Gui::MenuItem* join = new Gui::MenuItem;
    join->setCommand("Join");
    *join << "Part_JoinConnect"
          << "Part_JoinEmbed"
          << "Part_JoinCutout";

    Gui::MenuItem* split = new Gui::MenuItem;
    split->setCommand("Split");
    *split << "Part_BooleanFragments"
           << "Part_SliceApart"
           << "Part_Slice"
           << "Part_XOR";

    Gui::MenuItem* compound = new Gui::MenuItem;
    compound->setCommand("Compound");
    *compound << "Part_Compound"
              << "Part_ExplodeCompound"
              << "Part_CompoundFilter"
              << "Part_ToleranceSet";

    Gui::MenuItem* part = new Gui::MenuItem;
    root->insertItem(item, part);
    part->setCommand("&Part");
    *part << "Part_Import"
          << "Part_Export"
          << "Separator"
          << "Part_BoxSelection"
          << "Separator";
    *part << prim
          << "Part_Primitives"
          << "Part_Builder"
          << "Separator"
          << "Part_ShapeFromMesh"
          << "Part_PointsFromMesh"
          << "Part_MakeSolid"
          << "Part_ReverseShape"
          << copy
          << "Part_CheckGeometry"
          << "Part_Defeaturing"
          << "Materials_InspectAppearance"
          << "Materials_InspectMaterial"
          << "Separator"
          << bop << join << split << compound
          << "Separator";
    if (hasSketcher) {
      *part << "Sketcher_NewSketch";
    }
      *part << "Part_Extrude"
          << "Part_Revolve"
          << "Part_Mirror"
          << "Part_Scale"
          << "Part_Fillet"
          << "Part_Chamfer"
          << "Part_MakeFace"
          << "Part_RuledSurface"
          << "Part_Loft"
          << "Part_Sweep"
          << "Part_Section"
          << "Part_CrossSections"
          << "Part_Offset"
          << "Part_Offset2D"
          << "Part_Thickness"
          << "Part_ProjectionOnSurface"
          << "Separator"
          << "Part_EditAttachment";

    Gui::MenuItem* view = root->findItem("&View");
    if (view) {
        Gui::MenuItem* appr = view->findItem("Std_RandomColor");
        appr = view->afterItem(appr);
        Gui::MenuItem* face = new Gui::MenuItem();
        face->setCommand("Part_ColorPerFace");
        view->insertItem(appr, face);
    }

    return root;
}

Gui::ToolBarItem* Workbench::setupToolBars() const
{
    Gui::ToolBarItem* root = StdWorkbench::setupToolBars();

    Gui::ToolBarItem* solids = new Gui::ToolBarItem(root);
    solids->setCommand("Solids");
    *solids << "Part_Box"
            << "Part_Cylinder"
            << "Part_Sphere"
            << "Part_Cone"
            << "Part_Torus"
            << "Part_Tube"
            << "Part_Primitives"
            << "Part_Builder";

    Gui::ToolBarItem* tool = new Gui::ToolBarItem(root);
    tool->setCommand("Part tools");
    if (hasSketcher) {
        *tool << "Sketcher_NewSketch";
    }
    *tool << "Part_Extrude"
          << "Part_Revolve"
          << "Part_Mirror"
          << "Part_Scale"
          << "Part_Fillet"
          << "Part_Chamfer"
          << "Part_MakeFace"
          << "Part_RuledSurface"
          << "Part_Loft"
          << "Part_Sweep"
          << "Part_Section"
          << "Part_CrossSections"
          << "Part_CompOffset"
          << "Part_Thickness"
          << "Part_ProjectionOnSurface"
          << "Part_ColorPerFace";  // See issues #0477 and #1954 in the tracker

    Gui::ToolBarItem* boolop = new Gui::ToolBarItem(root);
    boolop->setCommand("Boolean");
    *boolop << "Part_CompCompoundTools"
            << "Part_Boolean"
            << "Part_Cut"
            << "Part_Fuse"
            << "Part_Common"
            << "Part_CompJoinFeatures"
            << "Part_CompSplitFeatures"
            << "Part_CheckGeometry"
            << "Part_Defeaturing";

    return root;
}

Gui::ToolBarItem* Workbench::setupCommandBars() const
{
    // Part tools
    Gui::ToolBarItem* root = new Gui::ToolBarItem;
    return root;
}
