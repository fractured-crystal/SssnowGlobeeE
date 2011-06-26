/** 
 *
 * Copyright (c) 2009-2010, Kitty Barnett
 * 
 * The source code in this file is provided to you under the terms of the 
 * GNU General Public License, version 2.0, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 * PARTICULAR PURPOSE. Terms of the GPL can be found in doc/GPL-license.txt 
 * in this distribution, or online at http://www.gnu.org/licenses/gpl-2.0.txt
 * 
 * By copying, modifying or distributing this software, you acknowledge that
 * you have read and understood your obligations described above, and agree to 
 * abide by those obligations.
 * 
 */

#ifndef RLV_HANDLER_H
#define RLV_HANDLER_H

#include <stack>
#include "llagentconstants.h"
#include "llstartup.h"
#include "llviewerjointattachment.h"
#include "llviewerobject.h"
#include "lleventtimer.h"

#include "rlvcommon.h"
#include "rlvhelper.h"
#include "rlvlocks.h"

// ============================================================================

class RlvHandler
{
public:
	RlvHandler();
	~RlvHandler();

	// --------------------------------

	// NOTE: - to check @detach=n    -> (see RlvAttachmentLocks)
	//       - to check @addattach=n -> (see RlvAttachmentLocks)
	//       - to check @remattach=n -> (see RlvAttachmentLocks)
	//       - to check @addoutfit=n -> (see RlvWearableLocks)
	//       - to check @remoutfit=n -> (see RlvWearableLocks)
	//       - to check exceptions   -> isException()
public:
	// Returns TRUE is at least one object contains the specified behaviour (and optional option)
	bool hasBehaviour(ERlvBehaviour eBehaviour) const { return (eBehaviour < RLV_BHVR_COUNT) ? (0 != m_Behaviours[eBehaviour]) : false; }
	bool hasBehaviour(ERlvBehaviour eBehaviour, const std::string& strOption) const;
	// Returns TRUE if at least one object (except the specified one) contains the specified behaviour (and optional option)
	bool hasBehaviourExcept(ERlvBehaviour eBehaviour, const LLUUID& idObj) const;
	bool hasBehaviourExcept(ERlvBehaviour eBehaviour, const std::string& strOption, const LLUUID& idObj) const;

	// Adds or removes an exception for the specified behaviour
	void addException(const LLUUID& idObj, ERlvBehaviour eBhvr, const RlvExceptionOption& varOption);
	void removeException(const LLUUID& idObj, ERlvBehaviour eBhvr, const RlvExceptionOption& varOption);
	// Returns TRUE if the specified option was added as an exception for the specified behaviour
	bool isException(ERlvBehaviour eBhvr, const RlvExceptionOption& varOption, ERlvExceptionCheck typeCheck = RLV_CHECK_DEFAULT) const;
	// Returns TRUE if the specified behaviour should behave "permissive" (rather than "strict"/"secure")
	bool isPermissive(ERlvBehaviour eBhvr) const;

	#ifdef RLV_EXPERIMENTAL_COMPOSITEFOLDERS
	// Returns TRUE if the composite folder doesn't contain any "locked" items
	bool canTakeOffComposite(const LLInventoryCategory* pFolder) const;
	// Returns TRUE if the composite folder doesn't replace any "locked" items
	bool canWearComposite(const LLInventoryCategory* pFolder) const;
	// Returns TRUE if the folder is a composite folder and optionally returns the name
	bool getCompositeInfo(const LLInventoryCategory* pFolder, std::string* pstrName) const;
	// Returns TRUE if the inventory item belongs to a composite folder and optionally returns the name and composite folder
	bool getCompositeInfo(const LLUUID& idItem, std::string* pstrName, LLViewerInventoryCategory** ppFolder) const;
	// Returns TRUE if the folder is a composite folder
	bool isCompositeFolder(const LLInventoryCategory* pFolder) const { return getCompositeInfo(pFolder, NULL); }
	// Returns TRUE if the inventory item belongs to a composite folder
	bool isCompositeDescendent(const LLUUID& idItem) const { return getCompositeInfo(idItem, NULL, NULL); }
	// Returns TRUE if the inventory item is part of a folded composite folder and should be hidden from @getoufit or @getattach
	bool isHiddenCompositeItem(const LLUUID& idItem, const std::string& strItemType) const;
	#endif // RLV_EXPERIMENTAL_COMPOSITEFOLDERS

	// --------------------------------

	/*
	 * Helper functions 
	 */
public:
	// Accessors
	bool              getCanCancelTp() const		{ return m_fCanCancelTp; }					// @accepttp and @tpto
	void              setCanCancelTp(bool fAllow)	{ m_fCanCancelTp = fAllow; }				// @accepttp and @tpto
	const LLVector3d& getSitSource() const						{ return m_posSitSource; }		// @standtp
	void              setSitSource(const LLVector3d& posSource)	{ m_posSitSource = posSource; }	// @standtp

	// Command specific helper functions
	bool canShowHoverText(LLViewerObject* pObj) const;											// @showhovertext* command family
	bool canSit(LLViewerObject* pObj, const LLVector3& posOffset = LLVector3::zero) const;
	bool canStand() const;
	bool canTeleportViaLure(const LLUUID& idAgent) const;
	bool canTouch(const LLViewerObject* pObj, const LLVector3& posOffset = LLVector3::zero) const;	// @touch
	void filterChat(std::string& strUTF8Text, bool fFilterEmote) const;							// @sendchat, @recvchat and @redirchat
	bool redirectChatOrEmote(const std::string& strUTF8Test) const;								// @redirchat and @rediremote

	// Command processing helper functions
	ERlvCmdRet processCommand(const LLUUID& idObj, const std::string& strCommand, bool fFromObj);
	void       processRetainedCommands(ERlvBehaviour eBhvrFilter = RLV_BHVR_UNKNOWN, ERlvParamType eTypeFilter = RLV_TYPE_UNKNOWN);

	// Returns a pointer to the currently executing command (do *not* save this pointer)
	const RlvCommand* getCurrentCommand() const { return (!m_CurCommandStack.empty()) ? m_CurCommandStack.top() : NULL; }
	// Returns the UUID of the object we're currently executing a command for
	const LLUUID&     getCurrentObject() const	{ return (!m_CurObjectStack.empty()) ? m_CurObjectStack.top() : LLUUID::null; }

	// Initialization
	static BOOL canDisable();
	static BOOL isEnabled()	{ return m_fEnabled; }
	static BOOL setEnabled(BOOL fEnable);
protected:
	void clearState();

	// --------------------------------

	/*
	 * Event handling
	 */
public:
	void addBehaviourObserver(RlvBehaviourObserver* pBhvrObserver);
	void addCommandHandler(RlvCommandHandler* pHandler);
	void removeBehaviourObserver(RlvBehaviourObserver* pBhvrObserver);
	void removeCommandHandler(RlvCommandHandler* pHandler);
protected:
	void clearCommandHandlers();
	void notifyBehaviourObservers(const RlvCommand& rlvCmd, bool fInternal);
	bool notifyCommandHandlers(rlvCommandHandler f, const RlvCommand& rlvCmd, ERlvCmdRet& eRet, bool fNotifyAll) const;

	// Externally invoked event handlers
public:
	void onAttach(const LLViewerObject* pAttachObj, const LLViewerJointAttachment* pAttachPt);
	void onDetach(const LLViewerObject* pAttachObj, const LLViewerJointAttachment* pAttachPt);
	bool onGC();
	void onLoginComplete();
	void onSitOrStand(bool fSitting);
	static void onIdleStartup(void* pParam);

	/*
	 * Command processing
	 */
protected:
	ERlvCmdRet processCommand(const RlvCommand& rlvCmd, bool fFromObj);
	ERlvCmdRet processClearCommand(const RlvCommand& rlvCmd);

	// Command handlers (RLV_TYPE_ADD and RLV_TYPE_CLEAR)
	ERlvCmdRet processAddRemCommand(const RlvCommand& rlvCmd);
	ERlvCmdRet onAddRemAttach(const RlvCommand& rlvCmd, bool& fRefCount);
	ERlvCmdRet onAddRemDetach(const RlvCommand& rlvCmd, bool& fRefCount);
	ERlvCmdRet onAddRemSetEnv(const RlvCommand& rlvCmd, bool& fRefCount);
	// Command handlers (RLV_TYPE_FORCE)
	ERlvCmdRet processForceCommand(const RlvCommand& rlvCmd) const;
	ERlvCmdRet onForceRemAttach(const RlvCommand& rlvCmd) const;
	ERlvCmdRet onForceRemOutfit(const RlvCommand& rlvCmd) const;
	ERlvCmdRet onForceSit(const RlvCommand& rlvCmd) const;
	ERlvCmdRet onForceWear(const LLViewerInventoryCategory* pFolder, ERlvBehaviour eBhvr) const;
	// Command handlers (RLV_TYPE_REPLY)
	ERlvCmdRet processReplyCommand(const RlvCommand& rlvCmd) const;
	ERlvCmdRet onFindFolder(const RlvCommand& rlvCmd, std::string& strReply) const;
	ERlvCmdRet onGetAttach(const RlvCommand& rlvCmd, std::string& strReply) const;
	ERlvCmdRet onGetAttachNames(const RlvCommand& rlvCmd, std::string& strReply) const;
	ERlvCmdRet onGetInv(const RlvCommand& rlvCmd, std::string& strReply) const;
	ERlvCmdRet onGetInvWorn(const RlvCommand& rlvCmd, std::string& strReply) const;
	ERlvCmdRet onGetOutfit(const RlvCommand& rlvCmd, std::string& strReply) const;
	ERlvCmdRet onGetOutfitNames(const RlvCommand& rlvCmd, std::string& strReply) const;
	ERlvCmdRet onGetPath(const RlvCommand& rlvCmd, std::string& strReply) const;

	// --------------------------------

	/*
	 * Member variables
	 */
public:
	typedef std::map<LLUUID, RlvObject> rlv_object_map_t;
	typedef std::multimap<ERlvBehaviour, RlvException> rlv_exception_map_t;
protected:
	rlv_object_map_t      m_Objects;				// Map of objects that have active restrictions (idObj -> RlvObject)
	rlv_exception_map_t   m_Exceptions;				// Map of currently active restriction exceptions (ERlvBehaviour -> RlvException)
	S16                   m_Behaviours[RLV_BHVR_COUNT];

	rlv_command_list_t    m_Retained;
	RlvGCTimer*           m_pGCTimer;
	RlvWLSnapshot*        m_pWLSnapshot;

	std::stack<const RlvCommand*> m_CurCommandStack;// Convenience (see @tpto)
	std::stack<LLUUID>    m_CurObjectStack;			// Convenience (see @tpto)

	mutable std::list<RlvBehaviourObserver*> m_BhvrObservers;
	mutable std::list<RlvCommandHandler*>    m_CommandHandlers;

	static BOOL			  m_fEnabled;				// Use setEnabled() to toggle this

	bool                  m_fCanCancelTp;			// @accepttp and @tpto
	mutable LLVector3d    m_posSitSource;			// @standtp (mutable because onForceXXX handles are all declared as const)


	friend class RlvGCTimer;						// Timer clear its own point at destruction

	// --------------------------------

	/*
	 * Internal access functions used by unit tests
	 */
public:
	const rlv_object_map_t*    getObjectMap() const		{ return &m_Objects; }
	//const rlv_exception_map_t* getExceptionMap() const	{ return &m_Exceptions; }
};

typedef RlvHandler rlv_handler_t;
extern rlv_handler_t gRlvHandler;

// ============================================================================
// Inlined member functions
//

// Checked: 2009-10-04 (RLVa-1.0.4a) | Modified: RLVa-1.0.4a
inline void RlvHandler::addException(const LLUUID& idObj, ERlvBehaviour eBhvr, const RlvExceptionOption& varOption)
{
	m_Exceptions.insert(std::pair<ERlvBehaviour, RlvException>(eBhvr, RlvException(idObj, eBhvr, varOption)));
}

// Checked: 2010-03-27 (RLVa-1.2.0b) | Modified: RLVa-1.0.0f
inline bool RlvHandler::canShowHoverText(LLViewerObject *pObj) const
{
	return ( (!pObj) || (LL_PCODE_VOLUME != pObj->getPCode()) ||
		    !( (hasBehaviour(RLV_BHVR_SHOWHOVERTEXTALL)) ||
			   ( (hasBehaviour(RLV_BHVR_SHOWHOVERTEXTWORLD)) && (!pObj->isHUDAttachment()) ) ||
			   ( (hasBehaviour(RLV_BHVR_SHOWHOVERTEXTHUD)) && (pObj->isHUDAttachment()) ) ||
			   (isException(RLV_BHVR_SHOWHOVERTEXT, pObj->getID(), RLV_CHECK_PERMISSIVE)) ) );
}

// Checked: 2010-03-06 (RLVa-1.2.0c) | Added: RLVa-1.1.0j
inline bool RlvHandler::canSit(LLViewerObject* pObj, const LLVector3& posOffset /*= LLVector3::zero*/) const
{
	// The user can sit on the specified object if:
	//   - not prevented from sitting
	//   - not prevented from standing up or not currently sitting
	//   - not standtp restricted or not currently sitting (if the user is sitting and tried to sit elsewhere the tp would just kick in)
	//   - [regular sit] not @sittp=n or @fartouch=n restricted or if they clicked on a point within 1.5m of the avie's current position
	//   - [force sit] not @sittp=n restricted by a *different* object than the one that issued the command or the object is within 1.5m
	return
		( (pObj) && (LL_PCODE_VOLUME == pObj->getPCode()) ) &&
		(!hasBehaviour(RLV_BHVR_SIT)) && 
		( ((!hasBehaviour(RLV_BHVR_UNSIT)) && (!hasBehaviour(RLV_BHVR_STANDTP))) || 
		  ((gAgent.getAvatarObject()) && (!gAgent.getAvatarObject()->isSitting())) ) &&
		( ((NULL == getCurrentCommand() || (RLV_BHVR_SIT != getCurrentCommand()->getBehaviourType()))
			? ((!hasBehaviour(RLV_BHVR_SITTP)) && (!hasBehaviour(RLV_BHVR_FARTOUCH)))	// [regular sit]
			: (!hasBehaviourExcept(RLV_BHVR_SITTP, getCurrentObject()))) ||				// [force sit]
		  (dist_vec_squared(gAgent.getPositionGlobal(), pObj->getPositionGlobal() + LLVector3d(posOffset)) < 1.5f * 1.5f) );
}

// Checked: 2010-03-07 (RLVa-1.2.0c) | Added: RLVa-1.2.0a
inline bool RlvHandler::canStand() const
{
	// NOTE: return FALSE only if we're @unsit=n restricted and the avie is currently sitting on something and TRUE for everything else
	return (!hasBehaviour(RLV_BHVR_UNSIT)) || ((gAgent.getAvatarObject()) && (!gAgent.getAvatarObject()->isSitting()));
}

// Checked: 2010-12-11 (RLVa-1.2.2c) | Added: RLVa-1.2.2c
inline bool RlvHandler::canTeleportViaLure(const LLUUID& idAgent) const
{
	return ((!hasBehaviour(RLV_BHVR_TPLURE)) || (isException(RLV_BHVR_TPLURE, idAgent))) && (canStand());
}

// Checked: 2010-04-11 (RLVa-1.2.0e) | Modified: RLVa-1.1.0l
inline bool RlvHandler::canTouch(const LLViewerObject* pObj, const LLVector3& posOffset /*=LLVector3::zero*/) const
{
#ifdef RLV_EXTENSION_CMD_TOUCHXXX
	bool fCanTouch = (pObj) && 
		( (!hasBehaviour(RLV_BHVR_TOUCH)) || (!isException(RLV_BHVR_TOUCH, pObj->getRootEdit()->getID(), RLV_CHECK_PERMISSIVE)) );

	if (fCanTouch)
	{
		if ( (!pObj->isAttachment()) || (!pObj->permYouOwner()) )
		{
			// Rezzed prim or attachment worn by another avie
			fCanTouch = 
				( (!hasBehaviour(RLV_BHVR_TOUCHWORLD)) ||
				  (isException(RLV_BHVR_TOUCHWORLD, pObj->getRootEdit()->getID(), RLV_CHECK_PERMISSIVE)) ) &&
				( (!hasBehaviour(RLV_BHVR_FARTOUCH)) || 
				  (dist_vec_squared(gAgent.getPositionGlobal(), pObj->getPositionGlobal() + LLVector3d(posOffset)) <= 1.5f * 1.5f) );
		}
		else if (pObj->isHUDAttachment())
		{
			// HUD attachment
			fCanTouch = (!hasBehaviour(RLV_BHVR_TOUCHHUD)) || 
				(isException(RLV_BHVR_TOUCHHUD, pObj->getRootEdit()->getID(), RLV_CHECK_PERMISSIVE));
		}
		else
		{
			// Regular attachment worn by this avie
			fCanTouch = 
				( (!hasBehaviour(RLV_BHVR_TOUCHATTACH)) || 
				  (isException(RLV_BHVR_TOUCHATTACH, pObj->getRootEdit()->getID(), RLV_CHECK_PERMISSIVE)) );
		}
	}
	return fCanTouch;
#else
	return (pObj) &&
	  (
		((pObj->isAttachment()) && (pObj->permYouOwner())) ||
		( (!hasBehaviour(RLV_BHVR_FARTOUCH)) || 
		  (dist_vec_squared(gAgent.getPositionGlobal(), pObj->getPositionGlobal() + LLVector3d(posOffset)) <= 1.5f * 1.5f) )
	  );
#endif // RLV_EXTENSION_CMD_TOUCHXXX
}

inline bool RlvHandler::hasBehaviour(ERlvBehaviour eBehaviour, const std::string& strOption) const
{
	return hasBehaviourExcept(eBehaviour, strOption, LLUUID::null);
}

inline bool RlvHandler::hasBehaviourExcept(ERlvBehaviour eBehaviour, const LLUUID& idObj) const
{
	return hasBehaviourExcept(eBehaviour, std::string(), idObj);
}

inline bool RlvHandler::isPermissive(ERlvBehaviour eBhvr) const
{
	return (RlvCommand::hasStrictVariant(eBhvr)) 
		? !((hasBehaviour(RLV_BHVR_PERMISSIVE)) || (isException(RLV_BHVR_PERMISSIVE, eBhvr, RLV_CHECK_PERMISSIVE)))
		: true;
}

// Checked: 2009-10-04 (RLVa-1.0.4a) | Modified: RLVa-1.0.4a
inline void RlvHandler::removeException(const LLUUID& idObj, ERlvBehaviour eBhvr, const RlvExceptionOption& varOption)
{
	for (rlv_exception_map_t::iterator itException = m_Exceptions.lower_bound(eBhvr), 
			endException = m_Exceptions.upper_bound(eBhvr); itException != endException; ++itException)
	{
		if ( (itException->second.idObject == idObj) && (itException->second.varOption == varOption) )
		{
			m_Exceptions.erase(itException);
			break;
		}
	}
}

// Checked: 2009-11-25 (RLVa-1.1.0f) | Modified: RLVa-1.1.0f
inline ERlvCmdRet RlvHandler::processCommand(const LLUUID& idObj, const std::string& strCommand, bool fFromObj)
{
	if (STATE_STARTED != LLStartUp::getStartupState())
	{
		m_Retained.push_back(RlvCommand(idObj, strCommand));
		return RLV_RET_RETAINED;
	}
	return processCommand(RlvCommand(idObj, strCommand), fFromObj);
}

// ============================================================================

#endif // RLV_HANDLER_H
