
# This file was generated by the SOM Compiler.
# FileName: WPSAMF.sc.
# Generated using:
#     SOM Precompiler spc: 1.22
#     SOM Emitter emitcsc: 1.10

/*
 * 
 * 
 *   Module Name: WPSamFolder
 * 
 *   OS/2 Workplace Shell Access Manager
 * 
 *   Container for userdefined desktops
 * 
 */

#include "wpdesk.sc"

class: WPSamF, 
    external stem = WPSamF, external prefix = WPSamFolder_, classprefix = WPSamFM_, 
    major version = 1, minor version = 1, local, 
    file stem = WPSAMF;

    -- 
    --  CLASS: WPSamF
    -- 
    --  CLASS HIERARCHY:
    -- 
    --      SOMObject
    --        ��� WPObject
    --              ��� WPFileSystem
    --                    ��� WPFolder
    --                          ��� WPDesktop
    --                                ��� WPSamF
    -- 


parent class: WPDesktop;


release order:
    RootDesktop, clsQueryModuleHandle, clsIsShareware, 
    clsAbout;


passthru: C.h, after;

endpassthru;
methods:


    BOOL    RootDesktop();

    override wpInitData;

    -- From WPObject

    override wpSaveState;

    -- From WPObject

    override wpRestoreState;

    -- From WPObject

    override wpAddSettingsPages;

    -- From WPObject

    override wpFilterPopupMenu;

    -- From WPObject

    override wpModifyPopupMenu;

    -- From WPObject

    override wpMenuItemSelected;

    -- From WPObject

    override wpMenuItemHelpSelected;

    -- From WPObject

    override wpOpen;

    -- From WPObject

    HMODULE    clsQueryModuleHandle(), class;

    BOOL    clsIsShareware(), class;

    void    clsAbout(), class;

    override wpclsInitData, class;

    -- From WPObject

    override wpclsQueryTitle, class;

    -- From WPObject

