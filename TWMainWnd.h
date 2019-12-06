/* Copyright (C) 2001-2017 by Madhav Shanbhag and Eric Schmidt,
 * under the GNU General Public License. No warranty. See COPYING for details.
 */

#ifndef TWMAINWND_H
#define TWMAINWND_H


#include "ui_TWMainWnd.h"

#include "CCMetaData.h"

#include "generic.h"

#include "gen.h"
#include "defs.h"
#include "state.h"
#include "series.h"
#include "oshw.h"

#include <QMainWindow>

#include <QLocale>

class QSortFilterProxyModel;

class TileWorldMainWnd : public QMainWindow, protected Ui::TWMainWnd
{
	Q_OBJECT

public:
	enum Page
	{
		PAGE_GAME,
		PAGE_TABLE,
		PAGE_TEXT
	};

	TileWorldMainWnd(QWidget* pParent = 0, Qt::WindowFlags flags = 0);
	~TileWorldMainWnd();

	virtual bool eventFilter(QObject* pObject, QEvent* pEvent) override;
	virtual void closeEvent(QCloseEvent* pCloseEvent) override;

	bool SetKeyboardRepeat(bool bEnable);
	uint8_t* GetKeyState(int* pnNumKeys);
	int GetReplaySecondsToSkip() const;

	bool CreateGameDisplay();
	void ClearDisplay();
	bool DisplayGame(const gamestate* pState, int nTimeLeft, int nBestTime);
	int DisplayEndMessage(int nBaseScore, int nTimeScore, long lTotalScore, int nCompleted);
	int DisplayList(const char* szTitle, const tablespec* pTableSpec, int* pnIndex,
			DisplayListType eListType, int (*pfnInputCallback)(int*));
	int DisplayInputPrompt(const char* szPrompt, char* pInput, int nMaxLen,
			InputPromptType eInputType, int (*pfnInputCallback)());
	int GetSelectedRuleset();
	void SetSubtitle(const char* szSubtitle);

	void ReadExtensions(gameseries* pSeries);
	void Narrate(CCX::Text CCX::Level::*pmTxt, bool bForce = false);

	void ShowAbout();
	void SetPlayPauseButton(int p);

public slots:
	void HideVolumeWidget();

private slots:
	void OnListItemActivated(const QModelIndex& index);
	void OnFindTextChanged(const QString& sText);
	void OnFindReturnPressed();
	void OnRulesetSwitched(bool mschecked);
	void OnPlayback();
	void OnSpeedValueChanged(int nValue);
	void OnSpeedSliderReleased();
	void OnSeekPosChanged(int nValue);
	void OnTextNext();
	void OnTextPrev();
	void OnTextReturn();
	void OnCopyText();
	void OnMenuActionTriggered(QAction* pAction);

private:
	bool HandleEvent(QObject* pObject, QEvent* pEvent);
	void SetCurrentPage(Page ePage);
	void CheckForProblems(const gamestate* pState);
	void DisplayMapView(const gamestate* pState);
	void DisplayShutter();
	void SetSpeed(int nValue);
	void ReleaseAllKeys();
	void PulseKey(int nTWKey);
	int GetTWKeyForAction(QAction* pAction) const;
	void SetVolume(int volume);

	enum HintMode { HINT_EMPTY, HINT_TEXT, HINT_INITSTATE };
	bool SetHintMode(HintMode newmode);
	void SetScale(int s, bool checkPrevScale = true);

	bool m_bWindowClosed;

	Qt_Surface* m_pSurface;
	Qt_Surface* m_pInvSurface;
	TW_Rect m_disploc;

	double scale = 1;

	uint8_t m_nKeyState[TWK_LAST];

	struct MessageData{ QString sMsg; uint32_t nMsgUntil, nMsgBoldUntil; };
	QVector<MessageData> m_shortMessages;

	bool m_bKbdRepeatEnabled;

	int m_nRuleset;
	int m_nLevelNum;
	QString m_nLevelName;
	bool m_bProblematic;
	bool m_bOFNT;
	int m_nBestTime;
	HintMode m_hintMode;
	int m_nTimeLeft;
	bool m_bTimedLevel;
	bool m_bReplay;

	QSortFilterProxyModel* m_pSortFilterProxyModel;
	QLocale m_locale;

	CCX::Levelset m_ccxLevelset;

	QString m_sTextToCopy;

	QIcon playIcon;
	QIcon pauseIcon;

	QTimer *volTimer;
};


#endif
