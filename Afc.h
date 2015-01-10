#ifndef AFC_H
#define AFC_H

//AFC Version
#define AFC_VERSION		3.11
#define AFC_TYPE		3


//EditBox Notification
#define	EN_ENTER				WM_APP + 1

//Button States
#define BS_NORMAL		0x0001
#define BS_PUSHED		0x0002

//Button Styles
#define BS_CLICK		0x0010
#define BS_PUSH			0x0020
#define BS_CHECK		0x0040
#define BS_TAB			0x0080

#define BS_TRANSPARENT	0x0100
#define BS_INVISIBLE	0x0200
#define BS_IMAGE16		0x0400
#define BS_IMAGE24		0x0800

//Dropdown Menu Styles
#define DMS_DOWNWARD	0x0001
#define DMS_UPWARD		0x0002

#define DMS_IMAGE16		0x0004
#define DMS_IMAGE24		0x0008

//Button Messages								// 90 .. 139
#define BM_RELEASE				WM_APP + 90
#define BM_PUSH					WM_APP + 91
#define BM_HIGHLIGHT			WM_APP + 92
#define BM_UNHIGHLIGHT			WM_APP + 93
#define BM_IS_PUSHED			WM_APP + 94

#define BM_ADD_SUBBUTTON		WM_APP + 95
#define BM_REMOVE_SUBBUTTON		WM_APP + 96
#define BM_SELECT_SUBBUTTON		WM_APP + 97

#define BM_ADD_SUBIMAGE			WM_APP + 98
#define BM_REMOVE_SUBIMAGE		WM_APP + 99
#define BM_SELECT_SUBIMAGE		WM_APP + 100

#define BM_SET_PUSHED_IMAGE		WM_APP + 101
#define BM_SET_DISABLED_IMAGE	WM_APP + 102
#define BM_SET_TOOLTIP			WM_APP + 103
#define BM_SET_DROPDOWN_STYLE	WM_APP + 105

#define BM_ENABLE				WM_APP + 106
#define BM_DISABLE				WM_APP + 107

//ToolTip Messages								 
#define TTM_SHOW				WM_APP + 110
#define TTM_HIDE				WM_APP + 111

//Button Notification
#define BN_RELEASED				WM_APP + 120
#define BN_HIGHLIGHTED			WM_APP + 121


//ScrollBar Messages							// 140 .. 159
#define ASBM_SETHANDLE_SIZE		WM_APP + 140      
#define ASBM_GETHANDLE_SIZE		WM_APP + 141
      
#define ASBM_SETHANDLE_POS		WM_APP + 142
#define ASBM_GETHANDLE_POS		WM_APP + 143
#define ASBM_SET_RANGE			WM_APP + 144
#define ASBM_GET_RANGE			WM_APP + 145

//ScrollBar Notification
#define ASBN_SCROLLING			WM_APP + 20

//ScrollBar Variables
#define ScrlBarRange			0
#define ScrlBarHandleSize		4
#define ScrlBarHandlePos		8
#define ScrlBarHMOffset			12


//DialogBar Messages							// 160 .. 189
#define	DBM_DRAG_START			WM_APP + 160

//ControlGroup Messages
#define ACGM_OPEN				WM_APP + 170      
#define ACGM_CLOSE				WM_APP + 171
#define ACGM_SETCTRL_STATE		WM_APP + 172      
#define ACGM_GETCTRL_STATE		WM_APP + 173
#define ACGM_GET_DLG			WM_APP + 174
#define ACGM_SET_STATE			WM_APP + 175
#define ACGM_GET_STATE			WM_APP + 176

//ControlGroup Notification
#define ACGN_OPENED				WM_APP + 180      
#define ACGN_CLOSED				WM_APP + 181

//ControlGroup States
#define ACGS_NORMAL				1
#define ACGS_CLOSED				2

//ControlGroup Variables
#define CtrlGroupPosition		0
#define CtrlGroupState			4
#define CtrlGroupHeightOpen		8
#define CtrlGroupHeightClosed	12
#define CtrlGroupWidth			16
#define CtrlGroupDialog			20
#define CtrlGroupMargin			24
#define	CtrlGroupParent			28	


//TrackBar Messages								// 190 .. 239
#define ATBM_SETHANDLE_SIZE		WM_APP + 190      
#define ATBM_GETHANDLE_SIZE		WM_APP + 191
#define ATBM_SETHANDLE_POS		WM_APP + 192      
#define ATBM_GETHANDLE_POS		WM_APP + 193
#define ATBM_GET_HANDLE			WM_APP + 194
#define ATBM_SET_RANGE			WM_APP + 195
#define ATBM_GET_RANGE			WM_APP + 196

//TrackBar Notification
#define ATBN_SCROLLING			WM_APP + 200

//TrackBar Variables
#define TrackBarRangeStart		0
#define TrackBarRangeEnd		4
#define TrackBarHandleSize		8
#define TrackBarHandlePos		12
#define TrackBarHMOffset		16


//Spinner Variables
#define	AS_STATE_NORMAL			1
#define	AS_STATE_UP				2
#define	AS_STATE_DOWN			3
#define	AS_STATE_BOTH			4

//Spinner Messages								// 240 .. 259
#define ASM_UPDATE				WM_APP + 240
#define ASM_SET_VAL_INT			WM_APP + 241
#define ASM_GET_VAL_INT			WM_APP + 242

#define ASM_SET_VAL_FLOAT		WM_APP + 243
#define ASM_GET_VAL_FLOAT		WM_APP + 244


//ToolBar Styles
#define	TBS_HSCROLL				0x0001
#define	TBS_VSCROLL				0x0002
#define	TBS_STATIC_EDGE			0x0004

#define	TBS_MARGIN_TOP			0x0010
#define	TBS_MARGIN_BOTTOM		0x0020
#define	TBS_MARGIN_LEFT			0x0040
#define	TBS_MARGIN_RIGHT		0x0080

#define	TBS_ALIGN_TOP			0x0100
#define	TBS_ALIGN_BOTTOM		0x0200
#define	TBS_ALIGN_LEFT			0x0400
#define	TBS_ALIGN_RIGHT			0x0800

//ToolBar Messages								// 260 .. 269
#define	TBM_DRAG_START			WM_APP + 260


void	InitControlsAFC( HINSTANCE hInst );

struct tSpinnerData
{
	float		Value;
	float		Step;
	Float2	Range;

	POINT		MousePos;
	UINT		State;

	float		ValueStart;
	POINT		MousePosStart;
	BOOL		Dragging;
	BOOL		Pushed;

	HWND		hBuddy;
};


struct tTrackBarData
{
	UINT		Position;
	UINT		Range;
	int			RangeStart;
	int			RangeEnd;

	BOOL		Selected;
	int			SelectionStart;
	int			SelectionEnd;

	int			*pKeyPosition;
	UINT		KeyCount;
};

struct tSubButtonInfo
{
	UINT		ID;
	UINT		ImageIndex;
};

struct tButtonInfo
{
//Basic Data
	UINT			State;
	UINT			Style;
	SIZE			Size;

	UINT			ID;
	COLORREF		Color;		

	bool			HighLighted;
	bool			Enabled;

	UINT			ImageIndex;
	UINT			Group;

	HWND			hParent;
	HWND			hWnd;

//Extended State Images
	bool			HasPushedImage;
	UINT			PushedIndex;

	bool			HasDisabledImage;
	UINT			DisabledIndex;

//SubImage Extension 
	int				ActiveSubImage;
	UINT			*pSubImageIndex;
	UINT			SubImageCount;

//DropDown Menu Extension
	int				ActiveSubButton;
	int				LastSubButton;

	UINT			DropdownMenuStyle;
	bool			DropdownMenuVisible;

	UINT			SubButtonCount;
	tSubButtonInfo	*pSubButton;

//TooTip Information
	bool			ToolTipVisible;
	POINT			LastMousePos;

	HWND			hToolTip;
	bool			ShowToolTip;
};


struct tControlGroupInfo
{
	UINT		DialogRes;
	UINT		State;

	DLGPROC		DlgProc;
};

struct tGroupData
{
	UINT	ID;
	UINT	Position;
};

struct tDialogData
{
	UINT		Height;	
	int			Position;
	int			RealPos;
	int			HandlePos;

	HWND		hWnd;
	UINT		ID;
	DLGPROC		Proc;

	tGroupData	*pGroup;
	UINT		GroupCount;

	UINT		*pGroupOrdered;
	UINT		OrderedCount;
};



///////////////////////////////////////////////////////////////////	
////****			    CLASS: (ACustomUI)					   ****////
///////////////////////////////////////////////////////////////////	

class ACustomUI
{
public:

	ACustomUI();
	~ACustomUI();

	void	DrawColorSwatch( HDC *pHdc, int xPos, int yPos, int xSize, int ySize, COLORREF color, bool selected );
	void	DrawSeparator( HDC *pHdc, int xPos, int yPos, int size, bool horizontal );
	void	DrawStaticEdge( HDC *pHdc, int xPos, int yPos, int xSize, int ySize, UINT type );
	void	DrawImage16( HDC *pHdc, int xPos, int yPos, UINT index ); 
	void	DrawImage24( HDC *pHdc, int xPos, int yPos, UINT index ); 
	void	DrawMarker( HDC *pHdc, int xPos, int yPos );

	UINT	AddImages16( LPSTR BitmapPath, LPSTR BitmapPath_a );
	void	RemoveImage16( UINT index );
	void	SetButtonImage16( HWND dialog, UINT controlID, UINT imageIndex );

	UINT	AddImages24( LPSTR BitmapPath, LPSTR BitmapPath_a );
	void	RemoveImage24( UINT index );
	void	SetButtonImage24( HWND dialog, UINT controlID, UINT imageIndex );


	void	DrawADropdownMenu( HWND hButton, UINT style );
	void	DrawAGroupControlUI( HWND hWnd );
	void	DrawAGroupControlButton( HWND hWnd );
	void	DrawASpinner( HWND hWnd, UINT state );

	void	DrawAButton( HWND hWnd, tButtonInfo *pInfo );
	void	DrawATrackBar( HWND hWnd );
	void	DrawAScrollBar( HWND hWnd );
	void	DrawADialogBar( HWND hWnd );

	COLORREF	Color3DLight; 
	COLORREF	Color3DShadow; 
	COLORREF	Color3DFace; 
	COLORREF	Color3DDarkShadow; 
	COLORREF	Color3DLightShadow; 

	HPEN		Pen3DLight;
	HPEN		Pen3DShadow;
	HPEN		Pen3DFace;
	HPEN		Pen3DDarkShadow;
	HPEN		PenBlack;
	HPEN		PenWhite;
	HPEN		PenVoid;

	HBRUSH		Brush3DDarkShadow;
	HBRUSH		Brush3DLightShadow;
	HBRUSH		Brush3DFace;
	HBRUSH		Brush3DShadow;
	HBRUSH		BrushOrange;
	HBRUSH		BrushRed;
	HBRUSH		BrushBlack;
	HBRUSH		BrushWhite;
	HBRUSH		BrushVoid;
	HBRUSH		BrushToolTip;

	HFONT		FontArialSmall;
	HFONT		FontArialMedium;

	COLORREF	Color1;

	HDC			hMemoryDC;
	HBITMAP		hBitmap1;

	POINT		Point1;
	POINT		Point2;

	SIZE		Size1;
	SIZE		Size2;

	RECT		Rect1;	
	RECT		Rect2;
	
	CHAR		Buffer1[64];
	CHAR		Buffer2[64];

	
	PAINTSTRUCT PaintStruct;
	HDC			hdc;

	UINT		Style;

	HIMAGELIST	hImageList16;
	HIMAGELIST	hImageList24;

	HINSTANCE	hInstance;

private:

	tSubButtonInfo	*pMenuButton;
	UINT			MenuButtonCount;

	HWND			hMenuWindow;
	UINT			SelectedButton;


};


///////////////////////////////////////////////////////////////////	
////****			    CLASS: (AToolBar)					   ****////
///////////////////////////////////////////////////////////////////	

//Needs Size Notification ( WM_SIZE )
class AToolBar
{
public:

	AToolBar();
	~AToolBar();

//Dialog Functions
	void	AddDialog( UINT resId, DLGPROC procedure );
	void	RemoveDialog();
	void	SetDialogPosition( int position );
	HWND	GetDialog();
	int		GetPosition();

	HWND	GetControl( UINT id );

//Main Functions
	void	Create( HWND parent, HINSTANCE hInst, UINT style );
	void	SetBarPosition( UINT x, UINT y, UINT width, UINT height );
	HWND	GetWindow();
	UINT	GetStyle();


	POINT			DragStartMousePos;	
	int				DragStartPosition;	
	bool			Dragging;
	bool			Dragable;

	RECT			FullRect;
	RECT			ViewRect;


private:

	UINT			MarginTop;
	UINT			MarginBottom;
	UINT			MarginLeft;
	UINT			MarginRight;
	
	HINSTANCE		hInstance;
	HWND			hParent;
	HWND			hWnd;

	HWND			hDialog;
	DLGPROC			DialogProc;

	int				Position;
	UINT			Style;
};



///////////////////////////////////////////////////////////////////	
////****			    CLASS: (ADialogBar)					   ****////
///////////////////////////////////////////////////////////////////	

//Needs Size Notification ( WM_SIZE )
class ADialogBar
{
public:

	UINT CreatedCount;


	ADialogBar();
	~ADialogBar();

//Dialog Functions
	void	AddDialog( UINT resId, DLGPROC procedure );
	void	RemoveDialog( UINT index );
	void	SelectDialog( UINT index );	
	void	ResetCurrentDialog();
	void	SetDialogPosition( UINT index, int position );
	void	Initialize();

	HWND	GetDialog( UINT index );
	HWND	GetGroup( UINT dialog, UINT id );
	HWND	GetGroupDialog( UINT dialog, UINT id );
	bool	IsGroupDialog( UINT dialog, UINT id );
	HWND	GetGroupControl( UINT dialog, UINT id, UINT controlId );
	HWND	GetTabBar();
	HWND	GetScrollBar();

	UINT	GetCurrentDialog();
	UINT	GetDialogCount();

//Group Functions
	void	SelectGroup( UINT dialog, UINT id, DLGPROC dlgProc );
//	void	HideGroup( UINT dialog, UINT id );
	void	ClearDialogGroups( UINT dialog );
	void	ReArrangeGroups();

//Main Functions
	void	Create( HWND parent, HINSTANCE hInst, UINT scrollBarId );
	void	SetBarPosition( UINT x, UINT y, UINT width, UINT height );
	void	SetTabBar( UINT resId, DLGPROC procedure );


//DialogBar Variables
	HWND			hParent;
	HWND			hWnd;
	UINT			MarginSize;

//Drawing Variables
	RECT			Rect;
	RECT			DialogMargin;
	RECT			DialogRect;
	UINT			TabBarHeight;

//Dragging Variables
	POINT			DragStartMousePos;	
	int				DragStartPosition;	
	bool			Dragging;

	bool			Dragable;


	tDialogData		*pDialog;

private:

//ScrollBar Variables
	int				ScrollPosition;
	UINT			ScrollBarID;
	SIZE			ScrollBarSize;
	POINT			ScrollBarPos;
	HWND			hScrollBar;

//Dialog Variables
	UINT			DialogCount;
	UINT			DialogCurrent;

//TabBar Variables
	HWND			hTabBar;
	UINT			TabBarID;

	HINSTANCE		hInstance;
};

///////////////////////////////////////////////////////////////////	
////****			    CLASS: (ATrackBar)					   ****////
///////////////////////////////////////////////////////////////////	

//Needs Size Notification ( WM_SIZE )
class ATrackBar
{
public:

	ATrackBar();
	~ATrackBar();

	void	Create( HWND parent, HINSTANCE hInst );

	HWND	GetHandle();

	void	SetBarPosition( UINT x, UINT y, UINT width, UINT height );
	void	SetRange( int start, int end );

	void	SetHandlePos( int position );
	void	GetHandlePos();


	HWND			hWnd;
	HWND			hParent;
	RECT			Rect;

	UINT			MarginSize;
	HINSTANCE		hInstance;

	int				RangeStart;
	int				RangeEnd;
	int				Range;

	int				HandlePosition;
	UINT			HandleSize;
};


#endif


