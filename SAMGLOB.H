/*******************************************************************
   Allgemeine Definitionen f�r SAM, WPSAM und INITOS2
   OS/2 - System Access Manager Version 2.1 - 9.7.1994
 *******************************************************************/
/* Schl�ssel f�r CRYPT */
#define ULKEY1          0xa4877120
#define ULKEY2          0xaf07ec71

#include <cutil.h>
#include <magcalls.h>

#define TITLE     "OS/2 - Workplace Shell Access Manager"

/* Stringl�ngen sind incl. '\0' angegeben */
#define CCHMAXUSERNAME      CCHMAXPATHCOMP  /* Maximale L�nge des  Benutzernamens     */
#define CCHMAXPASSWORD      128             /* Maximale Pa�wortl�nge                  */
#define CCHMAXENVIRONMENT   256             /* Maximale L�nge eines Env. Eintrages    */
#define CCHMAXREGUSER       32              /* Maximale L�nge des Registrierernamens  */
#define CCHMAXREGPWD        32              /* Maximale L�nge des Registrierkennworts */
#define CCHMAXMSG           256             /* Maximale Messagel�nge (WinMessageBox)  */
#define CCHMAXRCLINE        4096            /* Maximale Zeilenl�nge im User.RC        */

/* Definitionen f�r erweiterte Attribute */
#define EA_LONGNAME       ".LONGNAME"
#define EA_TYPENAME       ".TYPE"
#define EA_TYPEWPSAM      "WPSam"
#define EA_TYPEWPSAC      "WPSac"
#define EA_PWDNAME        "WPSAM.PASSWORD"
#define EA_USERRESOURCE   "WPSAM.USERRESOURCE"
#define EA_SYSTEMRESOURCE "WPSAM.SYSTEMRESOURCE"
#define EA_USERPROFILE    "WPSAM.USERPROFILE"
#define EA_SYSTEMPROFILE  "WPSAM.SYSTEMPROFILE"
#define EA_ENVNAME        "WPSAM.ENVIRONMENT"
#define EA_REGISTERNAME   "WPSAM.REGISTER"
#define EA_PROTSHELL      "WPSAM.PROTSHELL"
#define EA_FLAGS          "WPSAM.FLAGS"
#define EA_MODIFYDATE     "WPSAM.MODIFYDATE"

#define CCHMAXTYPENAME    max (EA_TYPEWPSAM, EA_TYPEWPSAC)

/* Neue Environment-Variablen */
#define ENV_SAMWORKPLACE  "SAMWORKPLACE"        /* Workplaceshell */
#define ENV_SAMUSERS      "SAM"                 /* Pfad der Benutzer */

/* Title f�r MessageBox-Funktion */
#define MBTITLE_ERROR   -1

/* Variable f�r das Ablegen von Instanzdaten der Object Class */
#define ENV_USER_INI     "USER_INI"                  /* Environment-Variable */
#define ENV_SYSTEM_INI   "SYSTEM_INI"                /* Environment-Variable */

/* Semaphoren und Shared Memory */
#define CCHSHARE_INITOS2  65536                      /* Size of shared memory */
#define SHARE_INITOS2    "\\SHAREMEM\\WPSam.InitOS2" /* Shared-Memory Name   */
#define HEV_SAMMY        "\\SEM32\\WPSam.Sammy"      /* prfReset starten */
#define HEV_PRFRESETLOCK "\\SEM32\\WPSam.LockSammy"  /* prfReset blockieren */

/* Flags f�r die ulFlag-Variable */
#define OTHERPROTSHELL  0x0001          /* Other ProtShell              */
#define PWDENABLE       0x0002          /* Password enabled             */
#define RENENABLE       0x0004          /* Rename enabled               */
#define MOVEENABLE      0x0008          /* Move enabled                 */
#define COPYENABLE      0x0010          /* Copy enabled                 */
#define DELENABLE       0x0020          /* Delete enabled               */
#define DROPALLOW       0x0040          /* Allow drop into me           */
#define PRIVATESYS      0x0080          /* Private system profile       */
#define WPSDISABLED     0x0100          /* User-WPS disabled            */
#define SETUPPASSED     0x0200          /* wpSetup was running at least one time */
#define ININITOS2MENU   0x0400          /* User is visible in Init-OS/2 menu     */
#define CLOSEAPPL       0x0800          /* Close applications when leaving       */

/* Flags f�r ulActive in SHARE1 */
#define ACTIVE_INITOS2  0x00000001      /* InitOS2 ist aktiv */
#define ACTIVE_SAMMY    0x00000002      /* Sammy ist aktiv */
#define ACTIVE_WPSAM    0x00000004      /* WPSam ist aktiv */
#define ACTIVE_WPSAC    0x00000008      /* WPSac ist aktiv */

/* Struktur f�r Shared-Memory SHARE_INITOS2 */
typedef struct _SHARE1
    {
    PCHAR   pszRegFile;                 /* Die EAs von RegFile enthalten Reg.String */
    PCHAR   pszRootUserIni;
    PCHAR   pszRootSystemIni;
    ULONG   ulFlag_Root;                /* Flags des Rootdesktops */
    PCHAR   pszUserIni;                 /* Userprofile f�r prfReset */
    PCHAR   pszSystemIni;               /* Systemprofile f�r prfReset */
    PCHAR   pszEnvironment;             /* Environment f�r Shell */
    ULONG   ulFlag;                     /* Flags des startenden Users */
    ULONG   ulActive;                   /* Aktive Programme im System */
    HWND    hwndSammy;
    ULONG   ulMsgLogout;
    ULONG   ulMsgPrfReset;
    } SHARE1;
typedef SHARE1 *PSHARE1;

/* �bergabeparameter an und Erfolgsmeldung von thCreate */
typedef struct _USER
    {
    PCHAR       pszMyName;              /* User-Name */
    PCHAR       pszMyPath;              /* User-Name incl. Pfad */
    PCHAR       pszUserResource;        /* Resource-File */
    PCHAR       pszUserProfile;         /* UserProfile */
    PCHAR       pszSystemResource;      /* Resource-File */
    PCHAR       pszSystemProfile;       /* SystemProfile */
    PCHAR       pszDesktopName;         /* Name des Desktop-Verzeichnisses */
    RESULTCODES rcSuccess;
    ULONG       ulErrorMsg;             /* Message bei Fehler, sonst 0 */
    } USER;
typedef USER *PUSER;

/* Fehlercodes von thCreate in MAKEINI */
#define ERR_NOUSERRESOURCE      1       /* => IDS_NOUSERRESOURCE     */
#define ERR_NOSYSTEMRESOURCE    2       /* => IDS_NOSYSTEMRESOURCE   */
#define ERR_NOUSERINICREATED    3       /* => IDS_NOUSERINICREATED   */
#define ERR_NOSYSTEMINICREATED  4       /* => IDS_NOSYSTEMINICREATED */
#define ERR_NODESKINRC          5       /* => IDS_NODESKINRC         */
#define ERR_PATHTOOLONG         6       /* => IDS_PATHTOOLONG        */

/* Kartenleserunterst�tzung */
typedef struct _MAGNET
    {
    HWND      hwnd;                     /* Window Handle */
    USHORT    usDID;                    /* aktives Entry-Feld */
    PCHAR     pszReadData;              /* Puffer f�r (konvertierten) String */
    } MAGNET;

typedef MAGNET *PMAGNET;

APIRET16 (* APIENTRY16 pfnMagOpen) (PHMAG);
APIRET16 (* APIENTRY16 pfnMagClose) (HMAG);
APIRET16 (* APIENTRY16 pfnMagSetOperationMode) (HMAG, OPMODE);
APIRET16 (* APIENTRY16 pfnMagResetDevice) (HMAG);
APIRET16 (* APIENTRY16 pfnMagAbort) (HMAG);
APIRET16 (* APIENTRY16 pfnMagReadData) (HMAG, USHORT, PBYTE, PUSHORT);
APIRET16 (* APIENTRY16 pfnMagReadConfigStatus) (HMAG, PUSHORT);

BOOL MCInit (void);
BOOL MCFree (void);
BOOL MCOpen (void);
BOOL MCClose (void);
BOOL MCAbort (void);
BOOL MCStart (PMAGNET);
BOOL MCRead (PMAGNET);
void _Optlink thMCRead (void *);

/* Private Debug-Hilfsroutinen; k�nnen �ber DEBUG */
/* Compiler-Direktive ein/ausgeschaltet werden.   */
#ifdef DEBUG
  int   pmprintf(char *f, ...);
  extern ULONG ulDebugMask;
  extern CHAR  szDebug[256];        /* F�r Funktionen wie z.B. _ultoa() */

  /* Zahl der Bit-Shifts f�r SOM_*Level - Variable */
  #define SHFT_TRACE            0
  #define SHFT_WARN             2
  #define SHFT_ASSERT           4

  /* DEBUG-Maske */
  #define D_SOM     0x00000001      // WPS-Methoden
  #define D_SWT     0x00000002      // Desktop-Umschaltung
  #define D_CRT     0x00000004      // INI-Erzeugung
  #define D_SAM     0x00000008      // Sammy

  /* Hilfe-Text f�r DEBUG-Maske */
  #define D_TEXT_MASK   "\
D_SOM     WPS-Methoden                 0x00000001\n\
D_SWT     Desktop-Umschaltung          0x00000002\n\
D_CRT     INI-Erzeugung                0x00000004\n\
D_CRT     Sammy                        0x00000008\n\
"

  #define DebugS(m,title)         pmprintf (m&ulDebugMask?"%s| %s\n":"", CLASSNAME, title)
  #define DebugE(m,title,text)    pmprintf (m&ulDebugMask?"%s| %s => %s\n":"", CLASSNAME, title, text)
  #define DebugULd(m,title,s,ul)  pmprintf (m&ulDebugMask?"%s| %s => %s: %d\n":"", CLASSNAME, title, s, ul)
  #define DebugULx(m,title,s,ul)  pmprintf (m&ulDebugMask?"%s| %s => %s: 0x%X\n":"", CLASSNAME, title, s, ul)
#else
  #define DebugS(m,title)
  #define DebugE(m,title,text)
  #define DebugULd(m,title,s,ul)
  #define DebugULx(m,title,s,ul)
#endif /* DEBUG */

/*******************************************************************
   Allgemeine Definitionen f�r WPSAC
 *******************************************************************/
#define BTB_LIB2                "AUTHORIZ"      /* DLL f�r WPSac */

/* Stringl�ngen sind incl. '\0' angegeben */
#define CCHMAXLOGSTRING     256             /* Maximale L�nge des Login/Logout-Strings */
#define CCHMAXCARDID        CCHMAXPATHCOMP  /* Maximale L�nge der Karten-ID */

/* Neue Environment-Variablen */
#define ENV_REMOTE_PATH   "SAMREMOTEPATH"   /* Remote Pfad f�r Alias */
#define ENV_LOCAL_PATH    "SAMLOCALPATH"    /* Lokaler Pfad f�r Alias */
#define ENV_ALIAS_LIST    "SAMALIASLIST"    /* Name der Alias-Datei */

/* Semaphoren */
#define HEV_LOGOUT       "\\SEM32\\WPSac.Logout"     /* Ausloggen */

typedef struct _ALIASDATA
    {
    USHORT cb;                          /* Size of allocated area */
    ULONG  ulFlag;                      /* Flags */
    CHAR   szUser[CCHMAXPATHCOMP];      /* User */
    CHAR   szPassword[CCHMAXPASSWORD];  /* Pa�wort */
    CHAR   szID[CCHMAXCARDID];          /* Karten-ID */
    CHAR   szDesktop[CCHMAXUSERNAME];   /* Desktop-Container */
    CHAR   szSacUser[CCHMAXPATH];       /* Parser-File */
    CHAR   szSacGroup[CCHMAXPATH];      /* User-Gruppe */
    CHAR   szLogin[CCHMAXLOGSTRING];    /* Login-String */
    CHAR   szLogout[CCHMAXLOGSTRING];   /* Logout-String */
    } ALIASDATA;
typedef ALIASDATA *PALIASDATA;

/* ALIASDATA.ulFlag: g�ltige Strings */
#define ALIAS_USER      0x0001          /* szUser     */
#define ALIAS_PASSWORD  0x0002          /* szPassword */
#define ALIAS_ID        0x0004          /* szID       */
#define ALIAS_DESKTOP   0x0008          /* szDesktop  */
#define ALIAS_SAC_USER  0x0010          /* szSacUser  */
#define ALIAS_SAC_GROUP 0x0020          /* szSacGroup */
#define ALIAS_LOGIN     0x0040          /* szLogin    */
#define ALIAS_LOGOUT    0x0080          /* szLogout   */

