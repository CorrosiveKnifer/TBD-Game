#include "Powerup.h"
#include "Player_Entity.h"
#include "SceneManager.h"

bool C_PowerUp::positionIsReserved[9] = { false }; // reserve positions to avoid overlap


// C_PowerUp(b2World* world, b2Vec2 _worldPosition,unsigned int _PU_typeID, int _reserved_POS_ID)
//
// Description:	Creates a powerup type and reserves the position it was spawned in till it is collected.
//				
// @param	b2World*		Pointer to the world it belongs in and is simulated in
// @param	b2Vec2			The world position to spawn this powerup at.
// @param	unsigned int	The type of powerup to make.
// @param	int				The reserved position ID tag to avoid overlap spawning.
//
// 
// @return	C_PowerUp object
//
C_PowerUp::C_PowerUp(b2World* world, b2Vec2 _worldPosition,unsigned int _PU_typeID, int _reserved_POS_ID)
{
	myPosition = _reserved_POS_ID; // this powerups position.
	C_PowerUp::positionIsReserved[myPosition] = true; // reserve this position

	std::string bottom, top;
	switch (_PU_typeID)
	{
	case  1:
		bottom = "images/powerups/pu_fastRun_1.png";
		top = "images/powerups/pu_fastRun_2.png";
		myPowerupType = SPEED;
	break;
	case  2:
		bottom = "images/powerups/pu_railShot_1.png";
		top = "images/powerups/pu_railShot_2.png";
		myPowerupType = RAILSHOT;
		break;
	case  3:
		bottom = "images/powerups/pu_shield_1.png";
		top = "images/powerups/pu_shield_2.png";
		myPowerupType = SHIELD;
		break;
	case  4:
		bottom = "images/powerups/pu_tripleShot_1.png";
		top = "images/powerups/pu_tripleShot_2.png";
		myPowerupType = TRIPLESHOT;
		break;
	case  5:
		bottom = "images/powerups/pu_waterfall_1.png";
		top = "images/powerups/pu_waterfall_2.png";
		myPowerupType = WATERFALL;
		break;
	default:
		break;
	}
	
	Spr_PowerUp_Bottom = Renderer::GetInstance().CreateSprite(bottom);
	Spr_PowerUp_Bottom->setOrigin(Spr_PowerUp_Bottom->getTexture()->getSize().x / 2.0f, Spr_PowerUp_Bottom->getTexture()->getSize().y / 2.0f);

	Spr_PowerUp_Top = Renderer::GetInstance().CreateSprite(top);
	Spr_PowerUp_Top->setOrigin(Spr_PowerUp_Top->getTexture()->getSize().x / 2.0f, Spr_PowerUp_Top->getTexture()->getSize().y / 2.0f);

	myColor = sf::Color(255, 255, 255, 255);


	//box2d setup
	MyBox2d.DEF.type = b2_dynamicBody;
	MyBox2d.DEF.position.Set(_worldPosition.x / C_GlobalVariables::PPM, _worldPosition.y / C_GlobalVariables::PPM);  // use spawn points for this.
	MyBox2d.DEF.bullet = true;
	MyBox2d.SHAPE.m_radius = (Tx_PowerupBottom.getSize().x / 2.0f) / C_GlobalVariables::PPM;
	MyBox2d.FIX.shape = &MyBox2d.SHAPE;
	MyBox2d.FIX.density = 0.5f;
	MyBox2d.FIX.friction = 0.5f;
	MyBox2d.FIX.restitution = 0.9f;
	MyBox2d.FIX.restitutionThreshold = 0.4f;
	MyBox2d.FIX.userData.pointer = reinterpret_cast<uintptr_t>(this);
	//MyBox2d.FIX.filter.categoryBits = C_GlobalVariables::GetCategoryFor(playerID);
	//MyBox2d.FIX.filter.maskBits = C_GlobalVariables::GetLayerMaskFor(playerID);
	MyBox2d.BOD = world->CreateBody(&MyBox2d.DEF);
	MyBox2d.BOD->CreateFixture(&MyBox2d.FIX);
}

// Draw()
//
// Description:	Draws a powerup sprite
//				
// @param	
// 
// @return	NA
//
void C_PowerUp::Draw()
{
	// draw powerups
	Renderer::GetInstance().Draw(*Spr_PowerUp_Bottom);
	Renderer::GetInstance().Draw(*Spr_PowerUp_Top);
}

// Process(float dT)
//
// Description:	Updates a powerup sprite position, and including a flashing effect
//				
// @param	float	delta time
// 
// @return	NA
//
void C_PowerUp::Process(float dT)
{
	Spr_PowerUp_Bottom->setPosition(this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM);
	Spr_PowerUp_Top->setPosition(this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM);

	this->mf_Powerup_FlashingTimer += dT;
	if (this->myColor.a > 240)
	{
		this->mi_Powerup_FlashDirection = -1;
	}
	if (this->myColor.a < 15)
	{
		this->mi_Powerup_FlashDirection = 1;
	}
	myColor.a += this->mi_Powerup_FlashDirection * this->mf_Powerup_FlashingTimer * this->mf_Powerup_Flashing;
	Spr_PowerUp_Top->setColor(this->myColor);
}

// HandleHit(Entity* other)
//
// Description:	Reacts to collision with a player entity, applies the powerup to that player and then calls to be destroyed.
//				
// @param	Entity*	the entity this powerup has collided with
// 
// @return	NA
//
void C_PowerUp::HandleHit(Entity* other)
{
	reinterpret_cast<C_Player*>(other)->ApplyPowerUp(this->myPowerupType);

	C_PowerUp::positionIsReserved[this->myPosition] = false; // free up the reserved position for another PU.

	SceneManager::GetInstance().DestroyEntity(this);
}

// ~C_PowerUp()
//
// Description:	Destructor, delete physical body, and the sprites.
//				
// @param	
// 
// @return	NA
//
C_PowerUp::~C_PowerUp()
{
	MyBox2d.BOD->GetWorld()->DestroyBody(MyBox2d.BOD);

	delete Spr_PowerUp_Bottom;
	Spr_PowerUp_Bottom = 0;

	delete Spr_PowerUp_Top;
	Spr_PowerUp_Top = 0;
}