#include "ComponentArranger.h"
#include "Entity.h"
#include "BoxCollisionComponent.h"
#include "RotatedBoxCollisionComponent.h"
#include "SpriteComponent.h"
#include "TransformableComponent.h"
#include "VelocityComponent.h"
#include "HealthComponent.h"
#include "Utility.h"
#include "GeneralData.h"
#include "AnimationComponent.h"
#include "ScriptUpdateComponent.h"
#include "AutomaticPathComponent.h"
#include "StaminaComponent.h"
#include "MeleeRectComponent.h"
#include "ScriptAIComponent.h"
#include "ScriptAITransition.h"
#include "LightPointComponent.h"
#include "CategoryComponent.h"
#include "CircularPathComponent.h"
#include "EntityParentComponent.h"
#include "AvoidanceBoxComponent.h"
#include "AttackModeComponent.h"
#include "TextDisplayComponent.h"
#include "HarmfulBoxesComponent.h"
#include "SpiritFormComponent.h"
#include "SpiritGrabberComponent.h"
#include "TimerComponent.h"


ComponentArranger::ComponentArranger(GeneralData* generalData)
:mTexturesStringManager(*generalData->getTexturesStringManager()),
mGeneralData(generalData)
{
}


ComponentArranger::~ComponentArranger()
{
}



void ComponentArranger::readFromLuaScript(Entity* entity,
	const std::string& scriptFilename, const std::string& tableName, 
	lua_State* luaState)
{
	if (luaL_dofile(luaState, scriptFilename.c_str()) != 0){
		std::cout << lua_tostring(luaState, -1) << std::endl;
		return;
	}
		
	luaL_openlibs(luaState);
	//if (luaL_dofile(luaState, scriptFilename.c_str()) != 0)
		//return;

	lua_pcall(luaState, 0, 0, 0);

	

	luabridge::LuaRef initialTableName = luabridge::getGlobal(
		luaState, tableName.c_str());
	//std::cout << luabridge::getGlobal(luaState, "test").cast<std::string>() << std::endl;
	if (initialTableName.isNil())
		return;

	luabridge::LuaRef table = initialTableName["initializeData"];
	if (!table[1].isNil())
		std::cout << "Not nil!" << std::endl;


	if (!table["SpriteComponent"].isNil() && table["SpriteComponent"].isTable())
		readSpriteComponent(entity, luaState, 
			table["SpriteComponent"].cast<luabridge::LuaRef>());
	
	if (!table["BoxCollisionComponent"].isNil() && table["BoxCollisionComponent"].isTable())
		readBoxCollisionComponent(entity, luaState, 
			table["BoxCollisionComponent"].cast<luabridge::LuaRef>());

	if (!table["TransformableComponent"].isNil() && table["TransformableComponent"].isTable())
		readTransformableComponent(entity, luaState,
			table["TransformableComponent"].cast<luabridge::LuaRef>());

	if (!table["HealthComponent"].isNil() && table["HealthComponent"].isTable())
		readHealthComponent(entity, luaState, 
			table["HealthComponent"].cast<luabridge::LuaRef>());

	if (!table["VelocityComponent"].isNil() && table["VelocityComponent"].isTable())
		readVelocityComponent(entity, luaState, 
			table["VelocityComponent"].cast<luabridge::LuaRef>());

	if (!table["AnimationComponent"].isNil() && table["AnimationComponent"].isTable())
		readAnimationComponent(entity, luaState,
			table["AnimationComponent"].cast<luabridge::LuaRef>());

	if (!table["ScriptUpdateComponent"].isNil() && table["ScriptUpdateComponent"].isTable())
		readScriptUpdateComponent(entity, luaState,
			table["ScriptUpdateComponent"].cast<luabridge::LuaRef>());

	if (!table["StaminaComponent"].isNil() && table["StaminaComponent"].isTable())
		readStaminaComponent(entity, luaState,
			table["StaminaComponent"].cast<luabridge::LuaRef>());

	if (!table["AutomaticPathComponent"].isNil() && table["AutomaticPathComponent"].isTable())
		readAutomaticPathComponent(entity, luaState,
			table["AutomaticPathComponent"].cast<luabridge::LuaRef>());

	if (!table["MeleeRectComponent"].isNil() && table["MeleeRectComponent"].isTable())
		readMeleeRectComponent(entity, luaState,
			table["MeleeRectComponent"].cast <luabridge::LuaRef>());

	if (!table["ScriptAIComponent"].isNil() && table["ScriptAIComponent"].isTable())
		readScriptAIComponent(entity, luaState, 
			table["ScriptAIComponent"].cast<luabridge::LuaRef>());

	if (!table["LightPointComponent"].isNil() && table["LightPointComponent"].isTable())
		readLightPointComponent(entity, luaState,
			table["LightPointComponent"].cast<luabridge::LuaRef>());

	if (!table["CategoryComponent"].isNil() && table["CategoryComponent"].isTable())
		readCategoryComponent(entity, luaState,
			table["CategoryComponent"].cast<luabridge::LuaRef>());

	if (!table["CircularPathComponent"].isNil() && table["CircularPathComponent"].isTable())
		readCircularPathComponent(entity, luaState,
			table["CircularPathComponent"].cast<luabridge::LuaRef>());

	if (!table["AvoidanceBoxComponent"].isNil() && table["AvoidanceBoxComponent"].isTable())
		readAvoidanceBoxComponent(entity, luaState,
			table["AvoidanceBoxComponent"].cast<luabridge::LuaRef>());

	if (!table["AttackModeComponent"].isNil() && table["AttackModeComponent"].isTable())
		readAttackModeComponent(entity, luaState,
			table["AttackModeComponent"].cast<luabridge::LuaRef>());

	if (!table["TextDisplayComponent"].isNil() && table["TextDisplayComponent"].isTable())
		readTextDisplayComponent(entity, luaState,
			table["TextDisplayComponent"].cast<luabridge::LuaRef>());

	if (!table["HarmfulBoxesComponent"].isNil() && table["HarmfulBoxesComponent"].isTable())
		readHarmfulBoxesComponent(entity, luaState,
			table["HarmfulBoxesComponent"].cast<luabridge::LuaRef>());

	if (!table["SpiritFormComponent"].isNil() && table["SpiritFormComponent"].isTable())
		readSpiritFormComponent(entity, luaState,
			table["SpiritFormComponent"].cast<luabridge::LuaRef>());

	if (!table["SpiritGrabberComponent"].isNil() && table["SpiritGrabberComponent"].isTable())
		readSpiritGrabberComponent(entity, luaState,
			table["SpiritGrabberComponent"].cast<luabridge::LuaRef>());

	if (!table["RotatedBoxCollisionComponent"].isNil() && table["RotatedBoxCollisionComponent"].isTable())
		readRotatedBoxCollisionComponent(entity, luaState,
			table["RotatedBoxCollisionComponent"].cast<luabridge::LuaRef>());

	if (!table["ChildEntityList"].isNil() && table["ChildEntityList"].isTable())
		arrangeChildEntityList(entity, luaState,
			table["ChildEntityList"].cast<luabridge::LuaRef>());
	
	

	if (!initialTableName["initializeFunction"].isNil() && initialTableName["initializeFunction"].isFunction())
	{
		luabridge::LuaRef func = initialTableName["initializeFunction"];
		try{
			func(entity);
		}
		catch (luabridge::LuaException e){
			std::cout << e.what() << std::endl;
		}
	}
}

void ComponentArranger::arrangeChildEntityList(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	int index = 1;
	while (!table[index].isNil()){
		luabridge::LuaRef curIndex = table[index];
		std::string tableName = curIndex["tableName"].cast<std::string>();
		std::string scriptDir = curIndex["scriptDir"].cast<std::string>();
		int posInd = 1;

		luabridge::LuaRef posListTable = curIndex["positionList"].cast<luabridge::LuaRef>();
		while (!posListTable[posInd].isNil()){
			Entity* childEntity = mGeneralData->getEntityManager()->createEntity("Sky");
			this->readFromLuaScript(childEntity, scriptDir, tableName, luaState);
			childEntity->comp<EntityParentComponent>()->mParent = entity;

			TransformableComponent* transformComp = childEntity->comp<TransformableComponent>();

			luabridge::LuaRef posTable = posListTable[posInd].cast<luabridge::LuaRef>();
			transformComp->setPositionX(posTable["positionX"].cast<float>());
			transformComp->setPositionY(posTable["positionY"].cast<float>());
			posInd++;
		}
		index++;
	}

}

void ComponentArranger::readCategoryComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	CategoryComponent* categoryComp = entity->comp<CategoryComponent>();
	categoryComp->setCategory(table["category"].cast<std::size_t>());
}

void ComponentArranger::readTransformableComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	TransformableComponent* transformComp = entity->comp<TransformableComponent>();

	if (!table["positionX"].isNil() && table["positionX"].isNumber())
		transformComp->setPositionX(table["positionX"].cast<float>());
	if (!table["positionY"].isNil() && table["positionY"].isNumber())
		transformComp->setPositionY(table["positionY"].cast<float>());
	
	if (!table["centerOriginX"].isNil() && table["centerOriginX"].isNumber())
		transformComp->setOrigin(table["centerOriginX"].cast<float>(), 
			transformComp->getOrigin().y);
	if (!table["centerOriginY"].isNil() && table["centerOriginY"].isNumber())
		transformComp->setOrigin(transformComp->getOrigin().x, 
			table["centerOriginY"].cast<float>());
}

void ComponentArranger::readSpriteComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	entity->comp<SpriteComponent>();
	if (!table["textureLocation"].isNil() && table["textureLocation"].isString()){
		/*std::cout << table["textureLocation"].cast<std::string>();
		mTexturesStringManager.loadTextures(table["textureLocation"].cast<std::string>());*/
		sf::Texture* texture = mTexturesStringManager.loadTextures(table["textureLocation"].cast<std::string>());
		if (texture){
			entity->comp<SpriteComponent>()->mSprite.setTexture(*texture);

			if (!table["textureRect"].isNil()){
				luabridge::LuaRef textureRectT = table["textureRect"].cast<luabridge::LuaRef>();
				entity->comp<SpriteComponent>()->mSprite.setTextureRect(sf::IntRect(
					textureRectT[1], textureRectT[2], textureRectT[3], textureRectT[4]));
			}

			/*bool isCentered = true;

			if (!table["centerOrigin"].isNil()){
				bool scriptCenter = table["centerOrigin"].cast<bool>();
				isCentered = scriptCenter;
			}

			if (isCentered) Utility::centerOrigin(entity->comp<SpriteComponent>()->mSprite);*/
		}
	}
}

void ComponentArranger::readBoxCollisionComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	BoxCollisionComponent* boxComp = entity->comp<BoxCollisionComponent>();
	if (!table["floatRect"].isNil() && table["floatRect"].isTable()){
		luabridge::LuaRef floatRectT = table["floatRect"].cast<luabridge::LuaRef>();
		
		boxComp->mBoundingRect = sf::FloatRect(
			floatRectT[1], floatRectT[2], floatRectT[3], floatRectT[4]);
	}

	if (!table["collisionReactor"].isNil() && table["collisionReactor"].isFunction()){
		luabridge::LuaRef function = table["collisionReactor"].cast<luabridge::LuaRef>();
		boxComp->setScriptCollisionReactor(function);
	}
}

void ComponentArranger::readHealthComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	HealthComponent* healthComp = entity->comp<HealthComponent>();

	if (!table["maxHealth"].isNil() && table["maxHealth"].isNumber())
		healthComp->setMaxHealth(table["maxHealth"].cast<float>());
	if (!table["currentHealth"].isNil() && table["currentHealth"].isNumber())
		healthComp->setCurrentHealth(table["currentHealth"].cast<float>());

}

void ComponentArranger::readVelocityComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	entity->comp<VelocityComponent>();
	if (!table["speed"].isNil() && table["speed"].isNumber())
		entity->comp<VelocityComponent>()->setSpeed(table["speed"].cast<float>());
	if (!table["sprintSpeed"].isNil() && table["sprintSpeed"].isNumber())
		entity->comp<VelocityComponent>()->setSprintSpeed(table["sprintSpeed"].cast<float>());
}


void ComponentArranger::readAnimationComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	std::string animScriptFileName = table["animScriptDir"].cast<std::string>();
	if (luaL_dofile(luaState, animScriptFileName.c_str()) != 0){
		std::cout << lua_tostring(luaState, -1) << std::endl;
		return;
	}

	lua_pcall(luaState, 0, 0, 0);

	luabridge::LuaRef animTable = luabridge::getGlobal(luaState, "Animations");


	entity->comp<AnimationComponent>();
	entity->comp<AnimationComponent>()->setTextureStringManager(&mTexturesStringManager);



	int i = 1;
	while (!animTable[i].isNil() && animTable[i].isTable()){
		luabridge::LuaRef curIndex = animTable[i].cast<luabridge::LuaRef>();
		AnimationData animData;
		
		std::string animName = curIndex["name"].cast<std::string>();
		animData.wholeDuration = sf::seconds(curIndex["wholeDuration"].cast<float>());
		animData.endToStart = curIndex["endToStart"].cast<bool>();
		animData.repeat = curIndex["isRepeating"].cast<bool>();
		animData.textureLocation = curIndex["textureLocation"].cast<std::string>();
		animData.flipX = curIndex["flipX"].cast<bool>();
		animData.transformOrigin = sf::Vector2f(-1.f, -1.f);

		if (!curIndex["transformOrigin"].isNil() && curIndex["transformOrigin"].isTable()){
			luabridge::LuaRef originTable = curIndex["transformOrigin"];

			animData.transformOrigin.x = originTable[1].cast<float>();
			animData.transformOrigin.y = originTable[2].cast<float>();
		}
		

		luabridge::LuaRef wholeFrameSizeTable = curIndex["wholeFrameSize"].cast<luabridge::LuaRef>();
		animData.wholeFrameSize = sf::Vector2i(wholeFrameSizeTable[1].cast<int>(), 
			wholeFrameSizeTable[2].cast<int>());


		int origIndex = 1;
		luabridge::LuaRef origListTable = curIndex["origLists"].cast<luabridge::LuaRef>();

		//Animation::OrigList origList;
		while (!origListTable[origIndex].isNil()){
			luabridge::LuaRef curOrigTable = origListTable[origIndex].cast<luabridge::LuaRef>();
			luabridge::LuaRef intRectT = curOrigTable["frame"].cast<luabridge::LuaRef>();

			animData.origList.push_back(std::make_pair(
				sf::IntRect(intRectT[1], intRectT[2], intRectT[3], intRectT[4]),
				sf::seconds(curOrigTable["duration"].cast<float>())
				));

			origIndex++;
		}

		entity->comp<AnimationComponent>()->insertNewAnimData(animName, animData);
		i++;

	}
		
}

void ComponentArranger::readAutomaticPathComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	entity->comp<AutomaticPathComponent>();
}

void ComponentArranger::readTextDisplayComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	entity->comp<TextDisplayComponent>();
}

void ComponentArranger::readStaminaComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	StaminaComponent* staminaComp = entity->comp<StaminaComponent>();
	staminaComp->setMaxStamina(table["maxStamina"].cast<float>());
	staminaComp->setCurStamina(staminaComp->getMaxStamina());
	staminaComp->setMaxStaminaRegen(table["maxStaminaRegen"].cast<float>());
	staminaComp->setStaminaRegen(staminaComp->getMaxStaminaRegen());
}

void ComponentArranger::readScriptUpdateComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	ScriptUpdateComponent* scriptComponent = entity->comp<ScriptUpdateComponent>();
	
	scriptComponent->setScriptUpdateFunc(table["updateFunc"], mGeneralData->getLuaState());
	scriptComponent->setScriptMemberVariables(table["memberVariables"]);
}


void ComponentArranger::readMeleeRectComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	MeleeRectComponent* meleeRectComponent = entity->comp<MeleeRectComponent>();

	
	int movementSetIndex = 1;

	if (!table["attackCategory"].isNil())
		meleeRectComponent->mAttackCategory = table["attackCategory"].cast<std::size_t>();

	luabridge::LuaRef movementSetsTable = table["movementSetList"].cast<luabridge::LuaRef>();

	while (!movementSetsTable[movementSetIndex].isNil()){
		MovementSetData movementSetData;
		luabridge::LuaRef curSetTable = movementSetsTable[movementSetIndex].cast<luabridge::LuaRef>();

		movementSetData.staminaUsage = curSetTable["staminaUsage"].cast<float>();
		movementSetData.damage = curSetTable["damage"].cast<float>();
		movementSetData.range = curSetTable["range"].cast<float>();
		movementSetData.thickness = curSetTable["thickness"].cast<float>();
		movementSetData.wholeDelayDur = sf::seconds(curSetTable["wholeDelayDuration"].cast<float>());
		movementSetData.wholeDuration = sf::seconds(curSetTable["wholeSwingDuration"].cast<float>());
		movementSetData.vulnerableDur = sf::seconds(curSetTable["vulnerableDuration"].cast<float>());
		movementSetData.recoveryDur = sf::seconds(curSetTable["recoveryDuration"].cast<float>());

		
		luabridge::LuaRef slashesTable = curSetTable["slashData"].cast <luabridge::LuaRef>();

		int slashIndex = 1;
		while (!slashesTable[slashIndex].isNil()){
			SlashData slashData(0.f, 0.f, 0);
			luabridge::LuaRef slashTable = slashesTable[slashIndex].cast<luabridge::LuaRef>();
			
			slashData.angle = slashTable["angle"].cast<float>();
			slashData.mDuration = sf::seconds(slashTable["swingDuration"].cast<float>());
			slashData.mDelayDuration = sf::seconds(slashTable["delayDuration"].cast<float>());

			luabridge::LuaRef movingPosT = slashTable["movingPos"].cast<luabridge::LuaRef>();
			
			slashData.movingPos = sf::Vector2f(movingPosT[1].cast<float>(), 
				movingPosT[2].cast<float>());;

			movementSetData.slashes.push_back(slashData);

			slashIndex++;
		}

		//int entityMovIndex = 1;
		if (!curSetTable["entityMovingData"].isNil()){
			luabridge::LuaRef entityMovingTable = curSetTable["entityMovingData"].cast<luabridge::LuaRef>();
			movementSetData.entityMovingData.speedIdentifier = entityMovingTable["speedIdentifier"];
		}

		meleeRectComponent->insertNewMovementSetData(curSetTable["name"].cast<std::string>(), movementSetData);
		movementSetIndex++;
	}
	//std::string haha = R"hahaha";
	if (!table["collisionReactor"].isNil() && table["collisionReactor"].isFunction()){
		luabridge::LuaRef function = table["collisionReactor"].cast<luabridge::LuaRef>();
		meleeRectComponent->setScriptCollisionReactor(function);
	}
}


void ComponentArranger::readScriptAIComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	std::string aiScriptFileName = table["AIScriptDir"].cast<std::string>();
	std::string frontName = table["tableName"].cast<std::string>();
	if (luaL_dofile(luaState, aiScriptFileName.c_str()) != 0){
		std::cout << lua_tostring(luaState, -1) << std::endl;
		return;
	}

	lua_pcall(luaState, 0, 0, 0);

	ScriptAIComponent* scriptAIComp = entity->comp<ScriptAIComponent>();
	scriptAIComp->mLuaState = luaState;
	
	luabridge::LuaRef states = luabridge::getGlobal(luaState, 
		std::string(frontName + "AIStates").c_str());

	if (!states["usePlayerFoundSystem"].isNil())
		scriptAIComp->mUsePlayerFoundSystem = states["usePlayerFoundSystem"].cast<bool>();
	if (!states["rangeCheckingToTarget"].isNil())
		scriptAIComp->mRangeCheckingToTarget = states["rangeCheckingToTarget"].cast<float>();

	//std::string initialStateName = states["initialState"].cast<std::string>();
	states["initialStateFunc"](entity);
}

void ComponentArranger::readLightPointComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	LightPointComponent* lightPointComp = entity->comp<LightPointComponent>();
	lightPointComp->createNewLightPoint(table["alwaysUpdate"].cast<bool>());
	ltbl::Light_Point* lightPoint = lightPointComp->getLightPoint();

	lightPoint->m_intensity = table["intensity"].cast<float>();
	lightPoint->m_radius = table["radius"].cast<float>();
	lightPoint->m_size = table["size"].cast<float>();
	lightPoint->m_spreadAngle = table["spreadAngle"].cast<float>();
	lightPoint->m_softSpreadAngle = table["softSpreadAngle"].cast<float>();
	lightPoint->CalculateAABB();
	lightPoint->m_bleed = table["bleed"].cast<float>();
	lightPoint->m_linearizeFactor = table["linearizeFactor"].cast<float>();
	
	mGeneralData->getLightSystem()->AddLight(lightPoint);
	lightPoint->SetAlwaysUpdate(table["alwaysUpdate"].cast<bool>());

	if (!table["maxRadius"].isNil() && table["maxRadius"].isNumber())
		lightPointComp->mMaxLightRadius = table["maxRadius"].cast<float>();
	if (!table["minRadius"].isNil() && table["minRadius"].isNumber())
		lightPointComp->mMinLightRadius = table["minRadius"].cast<float>();
}


void ComponentArranger::readCircularPathComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	CircularPathComponent* circularPathComp = entity->comp<CircularPathComponent>();
	if (!table["isRotatingAroundParentPos"].isNil())
	circularPathComp->setIsRotatingAroundParentPos(table["isRotatingAroundParentPos"].cast<bool>());
	
	if (!table["curRadius"].isNil() && table["curRadius"].isNumber())
		circularPathComp->setCurRadius(table["curRadius"].cast<float>());

	if (!table["centerPos"].isNil() && table["centerPos"].isTable())
		circularPathComp->setCenter(sf::Vector2f(
			table["centerPos"][1].cast<float>(), 
			table["centerPos"][2].cast<float>()));

	if (!table["speed"].isNil() && table["speed"].isNumber())
		circularPathComp->setSpeed(table["speed"].cast<float>());

	if (!table["clockWise"].isNil())
		circularPathComp->mIsClockwise = table["clockWise"].cast<bool>();

	if (!table["maxRadius"].isNil() && table["maxRadius"].isNumber())
		circularPathComp->mMaxRadius = table["maxRadius"].cast<float>();

	if (!table["minRadius"].isNil() && table["minRadius"].isNumber())
		circularPathComp->mMinRadius = table["minRadius"].cast<float>();
}


void ComponentArranger::readAvoidanceBoxComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	AvoidanceBoxComponent* avoidBoxComp = entity->comp<AvoidanceBoxComponent>();

	if (!table["rayLength"].isNil() && table["rayLength"].isNumber())
		avoidBoxComp->mRayLength = table["rayLength"].cast<float>();

	if (!table["rayThickness"].isNil() && table["rayThickness"].isNumber())
		avoidBoxComp->mRayThickness = table["rayThickness"].cast<float>();
}

void ComponentArranger::readAttackModeComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	AttackModeComponent* attackMode = entity->comp<AttackModeComponent>();

	if (!table["attackModeList"].isNil()){
		int index = 1;
		luabridge::LuaRef attackModeTable = table["attackModeList"];
		while (!attackModeTable[index].isNil()){
			attackMode->mAttackModeList.push_back(attackModeTable[index].cast<std::string>());
			index++;
		}
	}
}

void ComponentArranger::readHarmfulBoxesComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	HarmfulBoxesComponent* harmfulBoxesComp = entity->comp<HarmfulBoxesComponent>();

	int boxIndex = 1;
	luabridge::LuaRef boxesListTable = table["boxesList"].cast<luabridge::LuaRef>();

	while (!boxesListTable[boxIndex].isNil()){

		luabridge::LuaRef curBoxTable = boxesListTable[boxIndex].cast<luabridge::LuaRef>();

		luabridge::LuaRef floatRectT = curBoxTable["floatRect"].cast<luabridge::LuaRef>();
		
		harmfulBoxesComp->mHarmfulBoxesData.push_back(HarmfulBoxData());


		auto& harmfulBoxData = harmfulBoxesComp->mHarmfulBoxesData.back();


		harmfulBoxData.mBoxName = curBoxTable["name"].cast<std::string>();
		harmfulBoxData.mIsActivated = curBoxTable["activated"].cast<bool>();
		harmfulBoxData.mBoundingBox = 
			sf::FloatRect(floatRectT[1], floatRectT[2], floatRectT[3], floatRectT[4]);
		harmfulBoxData.mAttackCategory = curBoxTable["attackCategory"].cast<std::size_t>();
		harmfulBoxData.mHarmfulData.mDamage = curBoxTable["damage"].cast<std::size_t>();

		if (!curBoxTable["collisionReactor"].isNil())
			harmfulBoxData.mLuaCollisionReactor = std::make_shared<luabridge::LuaRef>
				(curBoxTable["collisionReactor"]);

		boxIndex++;
	}
}

void ComponentArranger::readSpiritFormComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	SpiritFormComponent* spiritComp = entity->comp<SpiritFormComponent>();

	spiritComp->mPassedThroughCategory = table["passedThroughCategory"].cast<std::size_t>();
	spiritComp->mDamageToPassedThroughCategory = table["damageToPassedThroughCategory"].cast<float>();
	spiritComp->mSpiritSpeed = table["spiritSpeed"].cast<float>();
	spiritComp->mNormalLifeTime = table["normalSpiritLifeTime"].cast<float>();
	spiritComp->mDelayBeforeSpirit = table["delayTime"].cast<float>();
	spiritComp->mStaminaIncrease = table["staminaIncrease"].cast<float>();
}

void ComponentArranger::readSpiritGrabberComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	SpiritGrabberComponent* spiritGrabberComp = entity->comp<SpiritGrabberComponent>();
}

void ComponentArranger::readRotatedBoxCollisionComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	RotatedBoxCollisionComponent* rotatedBoxComp = entity->comp<RotatedBoxCollisionComponent>();
	rotatedBoxComp->setScriptCollisionReactor(table["collisionReactor"].cast<luabridge::LuaRef>());


	luabridge::LuaRef rotatedRectT = table["rotatedRect"].cast<luabridge::LuaRef>();
	//first = left, second = top, third = width, fourth = height
	float firstIndex = rotatedRectT[1].cast<float>();
	float secondIndex = rotatedRectT[2].cast<float>();
	float thirdIndex = rotatedRectT[3].cast<float>();
	float fourthIndex = rotatedRectT[4].cast<float>();

	rotatedBoxComp->mRotatedRect.mPoints[RotatedRect::TopLeft] = 
		sf::Vector2f(firstIndex, secondIndex);
	rotatedBoxComp->mRotatedRect.mPoints[RotatedRect::TopLeft] = 
		sf::Vector2f(firstIndex + thirdIndex, secondIndex);
	rotatedBoxComp->mRotatedRect.mPoints[RotatedRect::TopLeft] = 
		sf::Vector2f(firstIndex + thirdIndex, secondIndex + fourthIndex);
	rotatedBoxComp->mRotatedRect.mPoints[RotatedRect::TopLeft] = 
		sf::Vector2f(firstIndex, secondIndex + secondIndex);
}

void ComponentArranger::readTimerComponent(Entity* entity, lua_State* luaState,
	luabridge::LuaRef& table)
{
	TimerComponent* timerComp = entity->comp<TimerComponent>();

	luabridge::LuaRef timerList = table["timerList"].cast<luabridge::LuaRef>();

	int index = 1;

	while (!timerList[index].isNil()){
		luabridge::LuaRef curTimer = timerList[index].cast<luabridge::LuaRef>();
		TimerData timerData;
		timerData.mElapsedTime = sf::seconds(curTimer["elapsedTime"].cast <float>());
		timerData.mTimeLimit = sf::seconds(curTimer["timeLimit"].cast <float>());
		timerComp->mTimerDataList.insert(std::make_pair(curTimer["name"].cast<std::string>(), timerData));

		index++;
	}
}