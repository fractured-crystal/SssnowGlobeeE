// <edit>
#include "llviewerprecompiledheaders.h"
#include "llkeytool.h"
#include "llagent.h"
#include "llcachename.h"
#include "llgroupmgr.h"
#include "lllandmark.h"
#include "llviewerobjectlist.h"
#include "lllocalinventory.h" // used by openKey
#include "llfloateravatarinfo.h" // used by openKey
#include "llfloatergroupinfo.h" // used by openKey
#include "llfloaterparcel.h" // used by openKey
#include "llchat.h" // used by openKey (region id...)
#include "llfloaterchat.h" // used by openKey (region id...)
#include "llfloaterworldmap.h" // used by openKey
#include "llselectmgr.h" // used by openKey
#include "llfloatertools.h" // used by openKey
#include "lltoolmgr.h" // used by openKey
#include "lltoolcomp.h" // used by openKey

std::list<LLKeyTool*> LLKeyTool::mKeyTools;
U32 LLKeyTool::sObjectPropertiesFamilyRequests = 0;
U32 LLKeyTool::sParcelInfoRequests = 0;
U32 LLKeyTool::sTransferRequests = 0;
U32 LLKeyTool::sImageRequests = 0;

LLKeyTool::LLKeyTool(LLUUID key, void (*callback)(LLUUID, LLKeyType, LLAssetType::EType, BOOL, void*), void* user_data)
{
	mKey = key;
	mCallback = callback;
	mKeyTools.push_back(this);
	mUserData = user_data;

	mObjectPropertiesFamilyRequests = 0;
	mParcelInfoRequests = 0;
	mTransferRequests = 0;
	mImageRequests = 0;

	tryAgent();
	tryTask();
	tryGroup();
	//tryRegion();
	tryParcel();
	tryItem();

	// try assets
	// it seems these come back in reverse order, so order them in reverse...
	std::list<LLAssetType::EType> ordered_asset_types;
	ordered_asset_types.push_back(LLAssetType::AT_BODYPART);
	ordered_asset_types.push_back(LLAssetType::AT_CLOTHING);
	ordered_asset_types.push_back(LLAssetType::AT_GESTURE);
	//ordered_asset_types.push_back(LLAssetType::AT_NOTECARD); // does not work anymore without an item
	ordered_asset_types.push_back(LLAssetType::AT_LANDMARK);
	ordered_asset_types.push_back(LLAssetType::AT_ANIMATION);
	ordered_asset_types.push_back(LLAssetType::AT_SOUND);
	ordered_asset_types.push_back(LLAssetType::AT_TEXTURE);
	for(std::list<LLAssetType::EType>::iterator iter = ordered_asset_types.begin();
		iter != ordered_asset_types.end();
		++iter)
	{
		tryAsset(*iter);
	}
}

LLKeyTool::~LLKeyTool()
{
	// Does this instance own all of the callbacks?
	if(mObjectPropertiesFamilyRequests >= sObjectPropertiesFamilyRequests)
	{
		sObjectPropertiesFamilyRequests = 0;
		mObjectPropertiesFamilyRequests = 0;
		gMessageSystem->delHandlerFuncFast(_PREHASH_ObjectPropertiesFamily, &onObjectPropertiesFamily);
	}
	if(mParcelInfoRequests >= sParcelInfoRequests)
	{
		sParcelInfoRequests = 0;
		mParcelInfoRequests = 0;
		gMessageSystem->delHandlerFuncFast(_PREHASH_ParcelInfoReply, &onParcelInfoReply);
	}
	if(mImageRequests >= sImageRequests)
	{
		sImageRequests = 0;
		mImageRequests = 0;
		gMessageSystem->delHandlerFuncFast(_PREHASH_ImageData, &onImageData);
		gMessageSystem->delHandlerFuncFast(_PREHASH_ImageNotInDatabase, &onImageNotInDatabase);
	}
	if(mTransferRequests >= sTransferRequests)
	{
		sTransferRequests = 0;
		mTransferRequests = 0;
		gMessageSystem->delHandlerFuncFast(_PREHASH_TransferInfo, &onTransferInfo);
	}
	// Remove from map
	mKeyTools.remove(this);
}

// static
std::string LLKeyTool::aWhat(LLKeyTool::LLKeyType key_type, LLAssetType::EType asset_type)
{
	std::string name = "Missingno.";
	std::string type = "Missingno.";
	switch(key_type)
	{
	case KT_AGENT:
		name = "agent";
		break;
	case KT_TASK:
		name = "task";
		break;
	case KT_GROUP:
		name = "group";
		break;
	case KT_REGION:
		name = "region";
		break;
	case KT_PARCEL:
		name = "parcel";
		break;
	case KT_ITEM:
		name = "item";
		break;
	case KT_ASSET:
		type = ll_safe_string(LLAssetType::lookupHumanReadable(asset_type));
		if (!type.empty())
		{
			name = type + " asset";
		}
		break;
	default:
		break;
	}
	return name;
}

// static
void LLKeyTool::openKey(LLUUID id, LLKeyType key_type, LLAssetType::EType asset_type)
{
	if(key_type == LLKeyTool::KT_ASSET)
	{
		LLLocalInventory::addItem(id.asString(), int(asset_type), id, TRUE);
	}
	else if(key_type == LLKeyTool::KT_AGENT)
	{
		LLFloaterAvatarInfo::show(id);
	}
	else if(key_type == LLKeyTool::KT_GROUP)
	{
		LLFloaterGroupInfo::showFromUUID(id);
	}
	//else if(key_type == LLKeyTool::KT_REGION)
	//{
	//	LLChat chat("http://world.secondlife.com/region/" + id.asString());
	//	LLFloaterChat::addChat(chat);
	//	gFloaterWorldMap->trackRegionID(id);
	//	LLFloaterWorldMap::show(NULL, TRUE);
	//}
	else if(key_type == LLKeyTool::KT_PARCEL)
	{
		LLFloaterParcelInfo::show(id);
	}
	else if(key_type == LLKeyTool::KT_ITEM)
	{
		LLLocalInventory::open(id);
	}
	else if(key_type == LLKeyTool::KT_TASK)
	{
		LLViewerObject* object = gObjectList.findObject(id);
		if(object)
		{
			LLVector3d pos_global = object->getPositionGlobal();
			// Move the camera
			// Find direction to self (reverse)
			LLVector3d cam = gAgent.getPositionGlobal() - pos_global;
			cam.normalize();
			// Go 4 meters back and 3 meters up
			cam *= 4.0f;
			cam += pos_global;
			cam += LLVector3d(0.f, 0.f, 3.0f);

			gAgent.setFocusOnAvatar(FALSE, FALSE);
			gAgent.setCameraPosAndFocusGlobal(cam, pos_global, id);
			gAgent.setCameraAnimating(FALSE);

			if(!object->isAvatar())
			{
				gFloaterTools->open();		/* Flawfinder: ignore */
				LLToolMgr::getInstance()->setCurrentToolset(gBasicToolset);
				gFloaterTools->setEditTool( LLToolCompTranslate::getInstance() );
				LLSelectMgr::getInstance()->selectObjectAndFamily(object, FALSE);
			}

		}
		else
		{
			// Todo: ObjectPropertiesFamily display
		}
	}
	else
	{
		llwarns << "Unhandled key type " << key_type << llendl;
	}
}

// static
BOOL LLKeyTool::callback(LLUUID id, LLKeyType key_type, LLAssetType::EType asset_type, BOOL is)
{
	BOOL wanted = FALSE;
	std::list<LLKeyTool*>::iterator kt_iter = mKeyTools.begin();
	std::list<LLKeyTool*>::iterator kt_end = mKeyTools.end();
	for( ; kt_iter != kt_end; ++kt_iter)
	{
		if((*kt_iter)->mKey == id)
		{
			LLKeyTool* tool = (*kt_iter);
			BOOL call = FALSE;

			if(key_type != KT_ASSET)
			{
				if(tool->mKeyTypesDone.find(key_type) == tool->mKeyTypesDone.end())
				{
					tool->mKeyTypesDone[key_type] = TRUE;
					call = TRUE;
				}
			}
			else // asset type
			{
				if(tool->mAssetTypesDone.find(asset_type) == tool->mAssetTypesDone.end())
				{
					tool->mAssetTypesDone[asset_type] = TRUE;
					call = TRUE;
				}
			}
			
			if(call)
			{
				tool->mKeyTypesDone[key_type] = is;
				llinfos << tool->mKey << (is ? " is " : " is not ") << aWhat(key_type, asset_type) << llendl;
				if(tool->mCallback)
				{
					tool->mCallback(id, key_type, asset_type, is, tool->mUserData);
					wanted = TRUE;
				}
				if(key_type == KT_TASK)
					tool->mObjectPropertiesFamilyRequests--;
				else if(key_type == KT_PARCEL)
					tool->mParcelInfoRequests--;
				else if(key_type == KT_ASSET)
				{
					if(asset_type == LLAssetType::AT_TEXTURE)
						tool->mImageRequests--;
					else
						tool->mTransferRequests--;
				}
			}
		}
	}
	return wanted;
}

void LLKeyTool::tryAgent()
{
	gCacheName->get(mKey, FALSE, onCacheName);
}

void LLKeyTool::onCacheName(const LLUUID& id, const std::string& first_name, const std::string& last_name, BOOL is_group, void* user_data)
{
	std::string agent_fail = "(\?\?\?)";
	if((!is_group) && (first_name != agent_fail))
		callback(id, LLKeyTool::KT_AGENT, LLAssetType::AT_NONE, TRUE);
	else
		callback(id, LLKeyTool::KT_AGENT, LLAssetType::AT_NONE, FALSE);
}

void LLKeyTool::tryTask()
{
	if(gObjectList.findObject(mKey))
	{
		callback(mKey, KT_TASK, LLAssetType::AT_NONE, TRUE);
	}
	else
	{
		if(sObjectPropertiesFamilyRequests <= 0)
		{
			// Prepare to receive ObjectPropertiesFamily packets
			// Note: no task = no reply
			sObjectPropertiesFamilyRequests = 0;
			gMessageSystem->addHandlerFuncFast(_PREHASH_ObjectPropertiesFamily, &onObjectPropertiesFamily);
		}
		gMessageSystem->newMessage(_PREHASH_RequestObjectPropertiesFamily);
		gMessageSystem->nextBlockFast(_PREHASH_AgentData);
		gMessageSystem->addUUIDFast(_PREHASH_AgentID, gAgent.getID());
		gMessageSystem->addUUIDFast(_PREHASH_SessionID, gAgent.getSessionID());
		gMessageSystem->nextBlockFast(_PREHASH_ObjectData);
		gMessageSystem->addU32Fast(_PREHASH_RequestFlags, 0);
		gMessageSystem->addUUIDFast(_PREHASH_ObjectID, mKey);
		gMessageSystem->sendReliable(gAgent.getRegionHost());
		sObjectPropertiesFamilyRequests++;
		mObjectPropertiesFamilyRequests++;
	}
}

// static
void LLKeyTool::onObjectPropertiesFamily(LLMessageSystem *msg, void **user_data)
{
	LLUUID id;
	msg->getUUIDFast(_PREHASH_ObjectData, _PREHASH_ObjectID, id);

	BOOL wanted = callback(id, KT_TASK, LLAssetType::AT_NONE, TRUE);
	if(wanted)
	{
		LLKeyTool::sObjectPropertiesFamilyRequests--;
		if(LLKeyTool::sObjectPropertiesFamilyRequests <= 0)
		{
			LLKeyTool::sObjectPropertiesFamilyRequests = 0;
			gMessageSystem->delHandlerFuncFast(_PREHASH_ObjectPropertiesFamily, &onObjectPropertiesFamily);
		}
		
	}
}

void LLKeyTool::tryGroup()
{
	LLGroupMgr::getInstance()->sendGroupPropertiesRequest(mKey);
}

// static
void LLKeyTool::gotGroupProfile(LLUUID id)
{
	callback(id, LLKeyTool::KT_GROUP, LLAssetType::AT_NONE, TRUE);
}

/*class LLKeyToolRegionHandleCallback : public LLRegionHandleCallback
{
public:
	LLKeyToolRegionHandleCallback() {}
	virtual ~LLKeyToolRegionHandleCallback() {}
	virtual bool dataReady(const LLUUID& region_id, const U64& region_handle)
	{
		if(region_handle == 0) LLKeyTool::callback(region_id, LLKeyTool::KT_REGION, LLAssetType::AT_NONE, FALSE);
		else LLKeyTool::callback(region_id, LLKeyTool::KT_REGION, LLAssetType::AT_NONE, TRUE);

		return false; // Still dunno what this bool is
	}
};*/

//void LLKeyTool::tryRegion()
//{
	//LLLandmark::requestRegionHandle(gMessageSystem, gAgent.getRegionHost(),
//		mKey, new LLKeyToolRegionHandleCallback());
//}

void LLKeyTool::tryParcel()
{
	if(sParcelInfoRequests <= 0)
	{
		// Prepare to receive ParcelInfoReply packets
		// Note: no parcel = no reply
		sParcelInfoRequests = 0;
		gMessageSystem->addHandlerFuncFast(_PREHASH_ParcelInfoReply, &onParcelInfoReply);
	}

	gMessageSystem->newMessage(_PREHASH_ParcelInfoRequest);
	gMessageSystem->nextBlockFast(_PREHASH_AgentData);
	gMessageSystem->addUUIDFast(_PREHASH_AgentID, gAgent.getID());
	gMessageSystem->addUUIDFast(_PREHASH_SessionID, gAgent.getSessionID());
	gMessageSystem->nextBlockFast(_PREHASH_Data);
	gMessageSystem->addUUIDFast(_PREHASH_ParcelID, mKey);
	gMessageSystem->sendReliable(gAgent.getRegionHost());
	sParcelInfoRequests++;
	mParcelInfoRequests++;
}

void LLKeyTool::onParcelInfoReply(LLMessageSystem *msg, void **user_data)
{
	LLUUID id;
	msg->getUUIDFast(_PREHASH_Data, _PREHASH_ParcelID, id);
	
	BOOL wanted = callback(id, KT_PARCEL, LLAssetType::AT_NONE, TRUE);
	if(wanted)
	{
		LLKeyTool::sParcelInfoRequests--;
		if(LLKeyTool::sParcelInfoRequests <= 0)
		{
			LLKeyTool::sParcelInfoRequests = 0;
			gMessageSystem->delHandlerFuncFast(_PREHASH_ParcelInfoReply, &onParcelInfoReply);
		}
	}
}

void LLKeyTool::tryItem()
{
	if(gInventory.getItem(mKey))
		callback(mKey, KT_ITEM, LLAssetType::AT_NONE, TRUE);
	else
		callback(mKey, KT_ITEM, LLAssetType::AT_NONE, FALSE);
}

void LLKeyTool::tryAsset(LLAssetType::EType asset_type)
{
	if(asset_type == LLAssetType::AT_TEXTURE)
	{
		if(sImageRequests <= 0)
		{
			// Prepare to receive ImageData ot ImageNotInDatabase packets
			sImageRequests = 0;
			gMessageSystem->addHandlerFuncFast(_PREHASH_ImageData, &onImageData);
			gMessageSystem->addHandlerFuncFast(_PREHASH_ImageNotInDatabase, &onImageNotInDatabase);
		}
		
		gMessageSystem->newMessageFast(_PREHASH_RequestImage);
		gMessageSystem->nextBlockFast(_PREHASH_AgentData);
		gMessageSystem->addUUIDFast(_PREHASH_AgentID, gAgent.getID());
		gMessageSystem->addUUIDFast(_PREHASH_SessionID, gAgent.getSessionID());
		gMessageSystem->nextBlockFast(_PREHASH_RequestImage);
		gMessageSystem->addUUIDFast(_PREHASH_Image, mKey);
		gMessageSystem->addS8Fast(_PREHASH_DiscardLevel, 0);
		gMessageSystem->addF32Fast(_PREHASH_DownloadPriority, 1015000);
		gMessageSystem->addU32Fast(_PREHASH_Packet, 0);
		gMessageSystem->addU8Fast(_PREHASH_Type, 0); // TYPE_NORMAL
		gMessageSystem->sendReliable(gAgent.getRegionHost());
		sImageRequests++;
		mImageRequests++;
	}
	else
	{
		if(sTransferRequests <= 0)
		{
			// Prepare to receive TransferInfo packets
			sTransferRequests = 0;
			gMessageSystem->addHandlerFuncFast(_PREHASH_TransferInfo, &onTransferInfo);
		}
		
		S32 type = (S32)(asset_type);
		LLUUID transfer_id;
		transfer_id.generate();
		U8 params[20];
		LLDataPackerBinaryBuffer dpb(params, 20);
		dpb.packUUID(mKey, "AssetID");
		dpb.packS32(type, "AssetType");

		gMessageSystem->newMessageFast(_PREHASH_TransferRequest);
		gMessageSystem->nextBlockFast(_PREHASH_TransferInfo);
		gMessageSystem->addUUIDFast(_PREHASH_TransferID, transfer_id);
		gMessageSystem->addS32Fast(_PREHASH_ChannelType, 2); // LLTCT_ASSET (e_transfer_channel_type)
		gMessageSystem->addS32Fast(_PREHASH_SourceType, 2); // LLTST_ASSET (e_transfer_source_type)
		gMessageSystem->addF32Fast(_PREHASH_Priority, 100.0f);
		gMessageSystem->addBinaryDataFast(_PREHASH_Params, params, 20);
		gMessageSystem->sendReliable(gAgent.getRegionHost());
		sTransferRequests++;
		mTransferRequests++;
	}
}

// static
void LLKeyTool::onTransferInfo(LLMessageSystem *msg, void **user_data)
{
	S32 params_size = msg->getSize(_PREHASH_TransferInfo, _PREHASH_Params);
	if(params_size < 1) return;
	U8 tmp[1024];
	msg->getBinaryDataFast(_PREHASH_TransferInfo, _PREHASH_Params, tmp, params_size);
	LLDataPackerBinaryBuffer dpb(tmp, 1024);
	LLUUID asset_id;
	dpb.unpackUUID(asset_id, "AssetID");
	S32 asset_type;
	dpb.unpackS32(asset_type, "AssetType");
	S32 status;
	msg->getS32Fast(_PREHASH_TransferInfo, _PREHASH_Status, status, 0);

	BOOL wanted = callback(asset_id, KT_ASSET, (LLAssetType::EType)asset_type, BOOL(status == 0)); // LLTS_OK (e_status_codes)
	if(wanted)
	{
		LLKeyTool::sTransferRequests--;
		if(LLKeyTool::sTransferRequests <= 0)
		{
			LLKeyTool::sTransferRequests = 0;
			gMessageSystem->delHandlerFuncFast(_PREHASH_TransferInfo, &onTransferInfo);
		}
	}
}

// static
void LLKeyTool::onImageData(LLMessageSystem *msg, void **user_data)
{
	LLUUID id;
	msg->getUUIDFast(_PREHASH_ImageID, _PREHASH_ID, id, 0);
	BOOL wanted = callback(id, KT_ASSET, LLAssetType::AT_TEXTURE, TRUE);
	if(wanted)
	{
		LLKeyTool::sImageRequests--;
		if(LLKeyTool::sImageRequests <= 0)
		{
			LLKeyTool::sImageRequests = 0;
			gMessageSystem->delHandlerFuncFast(_PREHASH_ImageData, &onImageData);
			gMessageSystem->delHandlerFuncFast(_PREHASH_ImageNotInDatabase, &onImageNotInDatabase);
		}
	}
}

// static
void LLKeyTool::onImageNotInDatabase(LLMessageSystem* msg, void **user_data)
{
	LLUUID id;
	msg->getUUIDFast(_PREHASH_ImageID, _PREHASH_ID, id, 0);
	BOOL wanted = callback(id, KT_ASSET, LLAssetType::AT_TEXTURE, FALSE);
	if(wanted)
	{
		LLKeyTool::sImageRequests--;
		if(LLKeyTool::sImageRequests <= 0)
		{
			LLKeyTool::sImageRequests = 0;
			gMessageSystem->delHandlerFuncFast(_PREHASH_ImageData, &onImageData);
			gMessageSystem->delHandlerFuncFast(_PREHASH_ImageNotInDatabase, &onImageNotInDatabase);
		}
	}
}

// </edit>