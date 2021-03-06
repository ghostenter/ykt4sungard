#if !defined(AFX_SMARTCOMMUNICATIONDLG_H__16698EB6_DF51_4966_9522_9D75263598C3__INCLUDED_)
#define AFX_SMARTCOMMUNICATIONDLG_H__16698EB6_DF51_4966_9522_9D75263598C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "systemtray.h"
#include "SmartLog.h"
#include "data.h"
/////////////////////////////////////////////////////////////////////////////
// CSmartCommunicationDlg dialog

class CSmartCommunicationDlg : public CDialog
{
// Construction
public:
	LRESULT OnTrayNotification(WPARAM wparam, LPARAM lparam);	
	CSmartCommunicationDlg(CWnd* pParent = NULL);	// standard constructor
	bool Init();
	void AddString(CString csAppend,bool bFlag=false);
	bool SMT_CalculateDevice();		//统计设备
	bool GetDevicePara();
	void WriteLog( char* msg);	
	void WriteLog( char* msg,char* sDeviceID );
	void ShowDeviceInfo();
	char *SetIPAddrToID( char * cAddrIP);	
	void SMT_GetAllFileName(vector<CString>&  VFileName);
	bool SMT_GetCurrFileName(char* cCurFileName,char* sBakFileName);
	void SMT_ReMoveFile(char* sFileName);
	void SMT_DeleteFile(char* sFileName);
	bool SMT_UpBeforeFile(XDHANDLE nHander);
	bool SMT_InsertOneTaskResult(SMARTTASKRESULT mTaskResult);	
	bool SMT_GetOneTaskResult(SMARTTASKRESULT *mTaskResult);
	bool SMT_ChnagLinkFileToUpFile(char *sYearMonth);
public:
	char	m_ErrMsg[256];
	char	m_CurPath[256];			//路径
	char	m_DealPath[256];		//交易明细保存路径
	unsigned int  nRateDeveice;		//水控设备
	CMyLog		  m_CLog;			//日志类
	CData		  m_Data;	
	char		m_ServerIP[30];		//服务端IP
	int			m_ServerPort;		//服务端口号
// Dialog Data
	//{{AFX_DATA(CSmartCommunicationDlg)
	enum { IDD = IDD_SMARTCOMMUNICATION_DIALOG };
	CButton	m_check_stop;
	CButton	m_check_depart;
	CEdit	m_id;
	CEdit	m_mac;
	CIPAddressCtrl	m_cAddress;
	CListBox	m_listResult;
	CEdit	m_edit_devicesuccess;
	CEdit	m_edit_devicerate;
	CEdit	m_edit_devicetotal;
	CButton	m_BtRate;
	CEdit	m_editResult;
	CButton	m_bol_show;
	CButton	m_BtStop;
	CButton	m_BtStart;
	CButton	m_BtClear;
	CButton	m_BtCancel;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmartCommunicationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
		afx_msg void OnButtonStop();
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSmartCommunicationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	virtual void OnCancel();
	afx_msg void OnButtonClear();
	afx_msg void OnFileUserExit();
	afx_msg void OnFileUserStop();
	afx_msg void OnFileUserStart();
	afx_msg void OnButtonSet();

	afx_msg void OnFileUserShow();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonUnitopen();
	afx_msg void OnButtonUnitclose();
	afx_msg void OnButtonLou();
	afx_msg void OnButtonLouclose();
	afx_msg void OnCheckDepart();
	afx_msg void OnCheckStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CSystemTray m_trayIcon;//定义一个托盘变量类型
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMARTCOMMUNICATIONDLG_H__16698EB6_DF51_4966_9522_9D75263598C3__INCLUDED_)
