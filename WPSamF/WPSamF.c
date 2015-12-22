/*
 * 
 * 
 *   Module Name: WPSamF
 * 
 *   OS/2 Workplace Shell Access Manager
 * 
 *   Container for userdefined desktops
 * 
 */


/*
 * This file was generated by the SOM Compiler and Bernhard Budnik
 * FileName: WPSamF.c.
 * Generated using:
 *     SOM Precompiler spc: 1.22
 *     SOM Emitter emitc: 1.24
 */



#define WPSamF_Class_Source
#define M_WPSamFM_Class_Source

#include "WPSamF.ih"
#include "WPSamF.ph"
#include "WPSamPan.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>


/*
 * Globale Daten, auf die von allen Instanzen und der Metaklasse
 * gemeinsam zugegriffen werden k�nnen.
 */

HMODULE hmod = NULLHANDLE;
CHAR    szSamFClassTitle[CCHMAXPATH];
PSHARE1 pShareInitOS2 = 0;
HEV     hevSammy = 0;
HEV     hevPrfResetLock = 0;


SOM_Scope void   SOMLINK WPSamFolder_wpInitData (WPSamF *somSelf)
{
    WPSamFData *somThis = WPSamFGetData (somSelf);
    WPSamFMethodDebug ("WPSamF","WPSamFolder_wpInitData");

    parent_wpInitData (somSelf); /* Parent method must be called prior own method */

    _ulZeitpunkt1 = _ulZeitpunkt2 = Hours ();
    _ulFlag       = SHUTDOWNENABLE | LOCKUPENABLE | ORIGDESKENABLE;
    _bCheckShare  = TRUE;

    DebugULx (D_SOM, "_wpInitData", "_ulZeitPunkt1", _ulZeitpunkt1);
}


SOM_Scope BOOL   SOMLINK WPSamFolder_wpRestoreState (WPSamF *somSelf,
                ULONG ulReserved)
{
    CHAR      *szClassName; /* Vor�bergehend zur Adaption */

    WPSamFData *somThis = WPSamFGetData (somSelf);
    WPSamFMethodDebug ("WPSamF","WPSamFolder_wpRestoreState");

    if (!parent_wpRestoreState (somSelf, ulReserved))
       return FALSE;

    szClassName = CLASSNAME;
    if (!_wpRestoreLong (somSelf, szClassName, KEY_FLAG, &_ulFlag))
       szClassName = "Desktop_User";

/*
 * In Zeitpunkt1 steht die Entstehungszeit des Objektes,
 * diese wird in _wpInitData festgelegt.
 * In Zeitpunkt2 steht die Zeit, wann er das letzte mal aktiver Desktop war,
 * diese wird jedesmal in _wpSaveState gespeichert.
 */

    _ulChiffreKey = (ULONG) _wpQueryHandle (somSelf);

    _wpRestoreLong (somSelf, szClassName, KEY_ZEITPUNKT2, &_ulZeitpunkt2);
    _wpRestoreLong (somSelf, szClassName, KEY_ZEITPUNKT1, &_ulZeitpunkt1);
    _wpRestoreLong (somSelf, szClassName, KEY_FLAG,       &_ulFlag);

    _ulZeitpunkt2 = _ulZeitpunkt2 ^ (_ulChiffreKey + 0x87654321);
    _ulZeitpunkt1 = _ulZeitpunkt1 ^ (_ulChiffreKey + _ulZeitpunkt2);

    return TRUE;
}


SOM_Scope BOOL   SOMLINK WPSamFolder_wpSaveState (WPSamF *somSelf)
{
    BOOL bRC = TRUE;

    WPSamFData *somThis = WPSamFGetData (somSelf);
    WPSamFMethodDebug ("WPSamF","WPSamFolder_wpSaveState");

    bRC &= parent_wpSaveState (somSelf);

    _ulChiffreKey = (ULONG) _wpQueryHandle (somSelf);

    bRC &= _wpSaveLong (somSelf, CLASSNAME, KEY_ZEITPUNKT2,
                        _ulZeitpunkt2 ^ (_ulChiffreKey + 0x87654321));

    bRC &= _wpSaveLong (somSelf, CLASSNAME, KEY_ZEITPUNKT1,
                        _ulZeitpunkt1 ^ (_ulChiffreKey + _ulZeitpunkt2));

    bRC &= _wpSaveLong (somSelf, CLASSNAME, KEY_FLAG, _ulFlag);

    if (bRC == FALSE)
       {
       DebugE (D_SOM, "_wpSaveState", "Failed!");
       MessageBox (IDS_SAVESTATEFAILED, MBTITLE_ERROR, MB_ENTER | MB_ERROR);
       }

    if (_wpIsCurrentDesktop (somSelf))
        {
        DosPostEventSem(hevPrfResetLock);
        DebugE (D_SOM, "_wpSaveState", "PrfReset unlocked");
        }

    return (bRC);
}


SOM_Scope BOOL   SOMLINK WPSamFolder_wpAddSettingsPages (WPSamF *somSelf,
                HWND hwndNotebook)
{
    PAGEINFO  pageinfo;
    CHAR      szPageName[CCHMAXPGNAME];

    /* WPSamFData *somThis = WPSamFGetData (somSelf); */
    WPSamFMethodDebug ("WPSamF","WPSamFolder_wpAddSettingsPages");

    memset ((PCH)&pageinfo,0,sizeof (PAGEINFO));
    WinLoadString (WinQueryAnchorBlock (HWND_DESKTOP), hmod,
                   IDS_WPSAMF_PG1, CCHMAXPGNAME, szPageName);
    pageinfo.cb                 = sizeof (PAGEINFO);
    pageinfo.hwndPage           = NULLHANDLE;
    pageinfo.usPageStyleFlags   = BKA_MAJOR;
    pageinfo.usPageInsertFlags  = BKA_FIRST;
    pageinfo.usSettingsFlags    = SETTINGS_PAGE_NUMBERS;
    pageinfo.pfnwp              = CtxtDlgProc;
    pageinfo.resid              = hmod;
    pageinfo.dlgid              = IDD_CONTEXTMENU;
    pageinfo.pszName            = szPageName;
    pageinfo.pCreateParams      = somSelf;
    pageinfo.idDefaultHelpPanel = PANEL_CONTEXTMENU;
    pageinfo.pszHelpLibraryName = HELPFILE;

    if (!_wpInsertSettingsPage (somSelf, hwndNotebook, &pageinfo))
       {
       DebugE (D_SOM, "_wpAddSettingsPages",
                "Failed to add Context_Menu settings page");
       return (FALSE);
       }

    if (!parent_wpAddSettingsPages (somSelf, hwndNotebook))
       {
       DebugE (D_SOM, "_wpAddSettingsPages",
                " Failed to add parent's settings pages");
       return (FALSE);
       }

    return TRUE;
}


SOM_Scope ULONG   SOMLINK WPSamFolder_wpFilterPopupMenu (WPSamF *somSelf,
                ULONG ulFlags,
                HWND hwndCnr,
                BOOL fMultiSelect)
{
    WPSamFData *somThis = WPSamFGetData (somSelf);
    ULONG      NewFlags;
    WPSamFMethodDebug ("WPSamF","WPSamFolder_wpFilterPopupMenu");

    NewFlags = parent_wpFilterPopupMenu (somSelf,ulFlags,hwndCnr,fMultiSelect);

    NewFlags &= ~ ((_ulFlag & SORTENABLE)     ? 0: CTXT_SORT)    &
                ~ ((_ulFlag & ARRANGEENABLE)  ? 0: CTXT_ARRANGE) &
                ~ ((_ulFlag & LOCKUPENABLE)   ? 0: CTXT_LOCKUP)  &
                ~ ((_ulFlag & SHUTDOWNENABLE) ? 0: CTXT_SHUTDOWN);
  
    return (NewFlags);
}


SOM_Scope BOOL   SOMLINK WPSamFolder_wpModifyPopupMenu (WPSamF *somSelf,
                HWND hwndMenu,
                HWND hwndCnr,
                ULONG iPosition)
{
    WPSamFData *somThis = WPSamFGetData (somSelf);
    WPSamFMethodDebug ("WPSamF","WPSamFolder_wpModifyPopupMenu");

    #if defined (DEBUG)
       _wpInsertPopupMenuItems (somSelf, hwndMenu, 0, hmod,
                                ID_OPENDEBUG, WPMENUID_PRIMARY);
    #endif

    if (_wpIsCurrentDesktop (somSelf) && (_ulFlag & ORIGDESKENABLE))
       _wpInsertPopupMenuItems (somSelf, hwndMenu, 0,
                                hmod, ID_ORIGDESK, WPMENUID_PRIMARY);

    return (parent_wpModifyPopupMenu (somSelf,hwndMenu,hwndCnr,iPosition));
}


SOM_Scope BOOL   SOMLINK WPSamFolder_wpMenuItemSelected (WPSamF *somSelf,
                HWND hwndFrame,
                ULONG ulMenuId)
{
    /* WPSamFData *somThis = WPSamFGetData (somSelf); */
    WPSamFMethodDebug ("WPSamF","WPSamFolder_wpMenuItemSelected");

    switch (ulMenuId)
       {
       case IDM_ORIGDESK:
          return (_wpViewObject (somSelf, NULLHANDLE, PRIMARY_ORIGDESK, 0) != NULLHANDLE);

       #ifdef DEBUG
       case IDM_OPENDEBUG:
             return (_wpViewObject (somSelf, hwndFrame, PRIMARY_DEBUG, 0) != NULLHANDLE);
       #endif

       default:
          return parent_wpMenuItemSelected (somSelf, hwndFrame, ulMenuId);
       }
}


SOM_Scope BOOL   SOMLINK WPSamFolder_wpMenuItemHelpSelected (WPSamF *somSelf,
                ULONG MenuId)
{
    /* WPSamFData *somThis = WPSamFGetData (somSelf); */
    WPSamFMethodDebug ("WPSamF","WPSamFolder_wpMenuItemHelpSelected");

   switch (MenuId)
   {
  /*  case IDM_OPENSHELL:    no help written at this time
   *
   *     return (_wpDisplayHelp (somSelf,ID_HELP_OPENSHELL,szHelpLibrary));
   *     break;
   */
      default:
         return (parent_wpMenuItemHelpSelected (somSelf,MenuId));
   }
}


SOM_Scope HWND   SOMLINK WPSamFolder_wpOpen (WPSamF *somSelf,
                HWND hwndCnr,
                ULONG ulView,
                ULONG param)
    {
    ULONG    ul;

    WPSamFData *somThis = WPSamFGetData (somSelf);
    WPSamFMethodDebug ("WPSamF","WPSamFolder_wpOpen");

    if (_wpIsCurrentDesktop (somSelf) && _bCheckShare)
        {
        SOMClass *Metaclass = SOM_GetClass (somSelf);

        DosResetEventSem(hevPrfResetLock, &ul);
        DebugULd (D_SOM, "_wpOpen", "hevPrfResetLock Lock count", ul);
        DebugE (D_SOM, "_wpOpen", "PrfReset locked");

        _bCheckShare = FALSE;
        DebugE (D_SOM, "_wpOpen", "clsIsShareware rufen");

        if (WPSamFM_clsIsShareware (Metaclass))
            {
            if (Hours () <  _ulZeitpunkt2 ||
                Hours () > (_ulZeitpunkt1 + 444) ||
                _ulZeitpunkt1 > _ulZeitpunkt2)
                {
                /* Shareware-Zeit abgelaufen */
                DebugE (D_SOM, "_wpOpen", "Shareware time expired");
                DebugULx (D_SOM, "_wpOpen", "_ulZeitPunkt1", _ulZeitpunkt1);
                DebugULx (D_SOM, "_wpOpen", "_ulZeitPunkt2", _ulZeitpunkt2);
                DebugULx (D_SOM, "_wpOpen", "Hours ()", Hours ());

                WPSamFM_clsAbout (Metaclass);

                if (WPSamFM_clsIsShareware (Metaclass))
                   {
                   DosResetEventSem(hevPrfResetLock, &ul);
                   DebugULd (D_SOM, "_wpOpen", "hevPrfResetLock Lock count", ul);
                   DebugE (D_SOM, "_wpOpen", "PrfReset locked");

                   _RootDesktop(somSelf);
                   }
                }
            else
                {
                /* Shareware-Zeit noch nicht abgelaufen */
                DebugE (D_SOM, "_wpOpen", "Shareware time isn't expired");

                _ulZeitpunkt2 = Hours ();
                _wpSaveDeferred (somSelf);
                }
            }
        }

    /* ok => Desktop kann ge�ffnet werden */

    switch (ulView)
        {
        #ifdef DEBUG
        case PRIMARY_DEBUG:
            DebugE (D_SOM, "wpsam_wpOpen", "Begin Debug");
                {
                }
            DebugE (D_SOM, "wpsam_wpOpen", "End Debug");
            break;
        #endif

        case PRIMARY_ORIGDESK:
            if (!_wpSwitchTo (somSelf, ulView))
               {
               DebugE (D_SOM, "_wpOpen", "Original Desktop");
               _RootDesktop (somSelf);
               }
            break;

        default:
            DebugE (D_SOM, "_wpOpen", "parent_wpOpen will be called");
            return parent_wpOpen (somSelf, hwndCnr, ulView,param);
        }

    DebugE (D_SOM, "_wpOpen", "Normal exit");

    return NULLHANDLE;   /* Nothing opened */
    }


SOM_Scope BOOL SOMLINK WPSamFolder_RootDesktop (WPSamF *somSelf)
    {
    static PRFPROFILE    prfProfile;
    PCHAR                szUserIni;
    PCHAR                szSysIni;
    /* WPSamFData *somThis = WPSamFGetData(somSelf); */
    WPSamFMethodDebug ("WPSamF","WPSamFolder_RootDesktop");

    /** Untersuchung, ob bereits im Rootdesktop **/
    /* Namen der laufenden User- und System-INI-Dateien */
    prfProfile.cchUserName = 0L;
    prfProfile.cchSysName  = 0L;

    PrfQueryProfile (WinQueryAnchorBlock(HWND_DESKTOP), &prfProfile);

    prfProfile.pszUserName = malloc (prfProfile.cchUserName);
    prfProfile.pszSysName  = malloc (prfProfile.cchSysName);

    PrfQueryProfile (WinQueryAnchorBlock(HWND_DESKTOP), &prfProfile);

    DebugE (D_SWT, "RootDesktop (current user-INI)",   prfProfile.pszUserName);
    DebugE (D_SWT, "RootDesktop (current system-INI)", prfProfile.pszSysName);

    /* Namen der Stammprofile */
    if (pShareInitOS2)
        {
        szUserIni = pShareInitOS2->pszRootUserIni;
        szSysIni  = pShareInitOS2->pszRootSystemIni;
        }
    else
        szUserIni = szSysIni = "";

    if (szUserIni[0] == '\0')
        if (DosScanEnv(USER_INI, &szUserIni))
            goto root_false;

    if (szSysIni[0] == '\0')
        if (DosScanEnv (SYSTEM_INI, &szSysIni))
            goto root_false;

    /* Vergleich der User-Ini Namen */
    if (strcmpi (szUserIni, prfProfile.pszUserName) == 0)
        {
        WinAlarm (HWND_DESKTOP, WA_NOTE);
        MessageBox (IDS_ALREADYINROOT, IDS_INFORMATION, MB_ENTER | MB_INFORMATION);
            goto root_false;
        }

    free (prfProfile.pszUserName);
    free (prfProfile.pszSysName);

    /** Umschaltung **/
    if (hevSammy)
        {
        pShareInitOS2->pszEnvironment[0] = '\0';
        pShareInitOS2->pszEnvironment[1] = '\0';
        pShareInitOS2->pszSystemIni[0]   = '\0';
        pShareInitOS2->pszUserIni[0]     = '\0';
        DebugE (D_SWT, "RootDesktop", "Ab hier macht Sammy weiter");
        DosPostEventSem (hevSammy);
        }

    return TRUE;

root_false:
    free (prfProfile.pszUserName);
    free (prfProfile.pszSysName);

    return FALSE;
    }

#undef SOM_CurrentClass
#define SOM_CurrentClass SOMMeta

SOM_Scope HMODULE   SOMLINK WPSamFM_clsQueryModuleHandle (M_WPSamF *somSelf)
{
    /* M_WPSamFData *somThis = M_WPSamFGetData (somSelf); */
    M_WPSamFMethodDebug ("M_WPSamF","WPSamFM_clsQueryModuleHandle");


    if(!hevPrfResetLock)
       if(DosOpenEventSem (HEV_PRFRESETLOCK, &hevPrfResetLock))
          if(DosCreateEventSem(HEV_PRFRESETLOCK, // Name
                                 &hevPrfResetLock, // Pointer to sem
                                 0,                // Not used with named sems
                                 TRUE))            // Initial state (TRUE = POSTED)
             hevPrfResetLock = 0;


    if(!pShareInitOS2)
       if (DosGetNamedSharedMem ((PPVOID) &pShareInitOS2,
                                        SHARE_INITOS2,
                                        PAG_READ | PAG_WRITE))
          pShareInitOS2 = 0;

    if(!hevSammy)
       if (DosOpenEventSem (HEV_SAMMY, &hevSammy))
          hevSammy = 0;


    if (hmod == NULLHANDLE)
    {
       zString zsPathName;

       /*
        *   Retrieve registered pathname of our module (DLL) and query the
        *   module handle.
        */
       zsPathName =
          _somLocateClassFile (SOMClassMgrObject, SOM_IdFromString (CLASSNAME),
                            WPSamF_MajorVersion, WPSamF_MinorVersion);

       if (DosQueryModuleHandle (zsPathName, &hmod))
       {
          DebugE (D_SOM, "_clsQueryModuleHandle", "Failed to load module");
          hmod = NULLHANDLE;
       }

    }   /* end if (hmod == NULLHANDLE) */

    DebugULd (D_SOM, "_clsQueryModuleHandle", "hmod", hmod);

    return (hmod);
}


SOM_Scope void   SOMLINK WPSamFM_wpclsInitData (M_WPSamF *somSelf)
{
    /* M_WPSamFData *somThis = M_WPSamFGetData (somSelf); */
    M_WPSamFMethodDebug ("M_WPSamF","WPSamFM_wpclsInitData");

    parent_wpclsInitData (somSelf);
    _clsQueryModuleHandle (somSelf);

    /* Get Class Title */
    if (!WinLoadString (WinQueryAnchorBlock (HWND_DESKTOP),
                        _clsQueryModuleHandle (somSelf),
                        IDS_TITLE_WPSAMF,
                        sizeof (szSamFClassTitle),
                        szSamFClassTitle))
        strcpy (szSamFClassTitle, _wpclsQueryTitle (somSelf));

    M_WPSamFMethodDebug ("M_WPSamF","WPSamFM_wpclsInitData Ende");

    return;
}


SOM_Scope PSZ   SOMLINK WPSamFM_wpclsQueryTitle (M_WPSamF *somSelf)
{
    /* M_WPSamFData *somThis = M_WPSamFGetData (somSelf); */
    M_WPSamFMethodDebug ("M_WPSamF","WPSamFM_wpclsQueryTitle");

    if (*szSamFClassTitle)
       return (szSamFClassTitle);
    else
       return (parent_wpclsQueryTitle (somSelf));
}


SOM_Scope BOOL   SOMLINK WPSamFM_clsIsShareware(M_WPSamF *somSelf)
    {
    SOMAny *WPSamClass;
    somId  idMethod;

    /* M_WPSamFData *somThis = M_WPSamFGetData(somSelf); */
    M_WPSamFMethodDebug ("M_WPSamF","WPSamFM_clsIsShareware");

    WPSamClass = _somFindClass (SOMClassMgrObject,
                                SOM_IdFromString ("WPSam"),
                                WPSamF_MajorVersion, WPSamF_MinorVersion);
    DebugULx (D_SOM, "_clsIsShareware", "WPSamClass", WPSamClass);

    idMethod = SOM_IdFromString ("clsIsShareware");
    DebugULd (D_SOM, "_clsIsShareware", "idMethod", idMethod);

    return (BOOL)_somDispatchL (WPSamClass, idMethod, (void *) 0);
    }


SOM_Scope void   SOMLINK WPSamFM_clsAbout(M_WPSamF *somSelf)
    {
    SOMAny *WPSamClass;
    somId  idMethod;

    /* M_WPSamFData *somThis = M_WPSamFGetData(somSelf); */
    M_WPSamFMethodDebug("M_WPSamF","WPSamFM_clsAbout");

    WPSamClass = _somFindClass (SOMClassMgrObject,
                                SOM_IdFromString ("WPSam"),
                                WPSamF_MajorVersion, WPSamF_MinorVersion);
    DebugULx (D_SOM, "_clsAbout", "WPSamClass", WPSamClass);

    idMethod = SOM_IdFromString ("clsAbout");
    DebugULd (D_SOM, "_clsAbout", "idMethod", idMethod);

    _somDispatchV (WPSamClass, idMethod, (void *) 0);

    return;
    }
