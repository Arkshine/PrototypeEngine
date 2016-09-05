/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef CONST_H
#define CONST_H
//
// Constants shared by the engine and dlls
// This header file included by engine files and DLL files.
// Most came from server.h

/**
*	edict->v.flags.
*/
enum EntFlag
{
	/**
	*	Changes the SV_Movestep() behavior to not need to be on ground.
	*/
	FL_FLY					= 1 << 0,

	/**
	*	Changes the SV_Movestep() behavior to not need to be on ground (but stay in water).
	*/
	FL_SWIM					= 1 << 1,
	FL_CONVEYOR				= 1 << 2,
	FL_CLIENT				= 1 << 3,
	FL_INWATER				= 1 << 4,
	FL_MONSTER				= 1 << 5,
	FL_GODMODE				= 1 << 6,
	FL_NOTARGET				= 1 << 7,

	/**
	*	Don't send entity to local host, it's predicting this entity itself.
	*/
	FL_SKIPLOCALHOST		= 1 << 8,

	/**
	*	At rest / on the ground
	*/
	FL_ONGROUND				= 1 << 9,

	/**
	*	Not all corners are valid.
	*/
	FL_PARTIALGROUND		= 1 << 10,

	/**
	*	Player jumping out of water.
	*/
	FL_WATERJUMP			= 1 << 11,

	/**
	*	Player is frozen for 3rd person camera.
	*/
	FL_FROZEN				= 1 << 12,

	/**
	*	JAC: fake client, simulated server side; don't send network messages to them.
	*/
	FL_FAKECLIENT			= 1 << 13,

	/**
	*	Player flag -- Player is fully crouched.
	*/
	FL_DUCKING				= 1 << 14,

	/**
	*	Apply floating force to this entity when in water.
	*/
	FL_FLOAT				= 1 << 15,

	/**
	*	Worldgraph has this ent listed as something that blocks a connection.
	*/
	FL_GRAPHED				= 1 << 16,

// UNDONE: Do we need these?
	FL_IMMUNE_WATER			= 1 << 17,
	FL_IMMUNE_SLIME			= 1 << 18,
	FL_IMMUNE_LAVA			= 1 << 19,

	/**
	*	This is a spectator proxy
	*/
	FL_PROXY				= 1 << 20,

	/**
	*	Brush model flag -- call think every frame regardless of nextthink - ltime (for constantly changing velocity/path).
	*/
	FL_ALWAYSTHINK			= 1 << 21,

	/**
	*	Base velocity has been applied this frame (used to convert base velocity into momentum).
	*/
	FL_BASEVELOCITY			= 1 << 22,

	/**
	*	Only collide in with monsters who have FL_MONSTERCLIP set.
	*/
	FL_MONSTERCLIP			= 1 << 23,

	/**
	*	Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
	*/
	FL_ONTRAIN				= 1 << 24,

	/**
	*	Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something).
	*/
	FL_WORLDBRUSH			= 1 << 25,

	/**
	*	This client is a spectator, don't run touch functions, etc.
	*/
	FL_SPECTATOR            = 1 << 26,

	/**
	*	This is a custom entity.
	*/
	FL_CUSTOMENTITY			= 1 << 29,

	/**
	*	This entity is marked for death -- This allows the engine to kill ents at the appropriate time
	*/
	FL_KILLME				= 1 << 30,

	/**
	*	Entity is dormant, no updates to client.
	*/
	FL_DORMANT				= 1 << 31,
};

/**
*	Goes into globalvars_t.trace_flags
*/
enum TraceFlag
{
	/**
	*	Traceline with a simple box.
	*/
	FTRACE_SIMPLEBOX	= 1 << 0
};

/**
*	Walkmove modes.
*/
enum WalkMove
{
	/**
	*	 Normal walkmove.
	*/
	WALKMOVE_NORMAL		= 0,

	/**
	*	Doesn't hit ANY entities, no matter what the solid type.
	*/
	WALKMOVE_WORLDONLY	= 1,

	/**
	*	Move, but don't touch triggers.
	*/
	WALKMOVE_CHECKONLY	= 2
};

/**
*	edict->v.movetype values.
*/
enum MoveType
{
	/**
	*	Never moves.
	*/
	MOVETYPE_NONE			= 0,
	//MOVETYPE_ANGLENOCLIP	= 1,
	//MOVETYPE_ANGLECLIP	= 2,

	/**
	*	Player only - moving on the ground.
	*/
	MOVETYPE_WALK			= 3,

	/**
	*	Gravity, special edge handling -- monsters use this.
	*/
	MOVETYPE_STEP			= 4,

	/**
	*	No gravity, but still collides with stuff.
	*/
	MOVETYPE_FLY			= 5,

	/**
	*	Gravity/collisions.
	*/
	MOVETYPE_TOSS			= 6,

	/**
	*	No clip to world, push and crush.
	*/
	MOVETYPE_PUSH			= 7,

	/**
	*	No gravity, no collisions, still do velocity/avelocity.
	*/
	MOVETYPE_NOCLIP			= 8,

	/**
	*	Extra size to monsters.
	*/
	MOVETYPE_FLYMISSILE		= 9,

	/**
	*	Just like Toss, but reflect velocity when contacting surfaces.
	*/
	MOVETYPE_BOUNCE			= 10,

	/**
	*	Bounce w/o gravity.
	*/
	MOVETYPE_BOUNCEMISSILE	= 11,

	/**
	*	Track movement of aiment.
	*/
	MOVETYPE_FOLLOW			= 12,

	/**
	*	BSP model that needs physics/world collisions (uses nearest hull for world collision).
	*/
	MOVETYPE_PUSHSTEP		= 13,
};

/**
*	edict->v.solid values.
*	NOTE: Some movetypes will cause collisions independent of SOLID_NOT/SOLID_TRIGGER when the entity moves.
*	SOLID only effects OTHER entities colliding with this one when they move - UGH!
*/
enum Solid
{
	/**
	*	No interaction with other objects.
	*/
	SOLID_NOT		= 0,

	/**
	*	Touch on edge, but not blocking.
	*/
	SOLID_TRIGGER	= 1,

	/**
	*	Touch on edge, block.
	*/
	SOLID_BBOX		= 2,

	/**
	*	Touch on edge, but not an onground.
	*/
	SOLID_SLIDEBOX	= 3,

	/**
	*	BSP clip, touch on edge, block.
	*/
	SOLID_BSP		= 4,
};

/**
*	edict->deadflag values.
*	Used by the engine, don't change.
*/
enum DeadFlag
{
	/**
	*	Alive.
	*/
	DEAD_NO				= 0,

	/**
	*	Playing death animation or still falling off of a ledge waiting to hit ground.
	*/
	DEAD_DYING			= 1,

	/**
	*	Dead. lying still.
	*/
	DEAD_DEAD			= 2,

	/**
	*	Dead, and can be respawned.
	*/
	DEAD_RESPAWNABLE	= 3,

	/**
	*	Not used in the SDK, used by TFC for spies feigning death.
	*/
	DEAD_DISCARDBODY	= 4,
};

/**
*	edict->v.takedamage values.
*/
enum TakeDamageMode
{
	/**
	*	Don't take damage.
	*/
	DAMAGE_NO	= 0,

	/**
	*	Take damage.
	*/
	DAMAGE_YES	= 1,

	/**
	*	Take damage, auto aim should snap to this target.
	*/
	DAMAGE_AIM	= 2,
};

/**
*	Entity effects.
*	edict->v.effects.
*/
enum Effect
{
	/**
	*	Swirling cloud of particles.
	*/
	EF_BRIGHTFIELD		= 1,

	/**
	*	Single frame ELIGHT on entity attachment 0.
	*/
	EF_MUZZLEFLASH 		= 2,

	/**
	*	DLIGHT centered at entity origin.
	*/
	EF_BRIGHTLIGHT 		= 4,

	/**
	*	Player flashlight.
	*/
	EF_DIMLIGHT 		= 8,

	/**
	*	Get lighting from ceiling.
	*/
	EF_INVLIGHT			= 16,

	/**
	*	Don't interpolate the next frame.
	*/
	EF_NOINTERP			= 32,

	/**
	*	Rocket flare glow sprite.
	*/
	EF_LIGHT			= 64,

	/**
	*	Don't draw entity.
	*/
	EF_NODRAW			= 128,

	/**
	*	Player nightvision.
	*/
	EF_NIGHTVISION		= 256,

	/**
	*	Sniper laser effect.
	*/
	EF_SNIPERLASER		= 512,

	/**
	*	Fiber camera.
	*/
	EF_FIBERCAMERA		= 1024,
};

/**
*	Constants for entvars_t::waterlevel
*	@see entvars_t::waterlevel
*/
enum WaterLevel
{
	/**
	*	Not in water at all.
	*/
	WATERLEVEL_DRY		= 0,

	/**
	*	Standing in water, feet only.
	*/
	WATERLEVEL_FEET		= 1,

	/**
	*	Halfway submerged.
	*/
	WATERLEVEL_WAIST	= 2,

	/**
	*	Submerged up to eyes or more.
	*/
	WATERLEVEL_HEAD		= 3
};

/**
*	Entity flags.
*	@see entity_state_t::eflags
*/
enum EntStateFlag
{
	/**
	*	Do studio interpolation of this entity.
	*/
	EFLAG_SLERP			= 1
};
		
//
// temp entity events
//
#define	TE_BEAMPOINTS		0		// beam effect between two points
// coord coord coord (start position) 
// coord coord coord (end position) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define	TE_BEAMENTPOINT		1		// beam effect between point and entity
// short (start entity) 
// coord coord coord (end position) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define	TE_GUNSHOT			2		// particle effect plus ricochet sound
// coord coord coord (position) 

#define	TE_EXPLOSION		3		// additive sprite, 2 dynamic lights, flickering particles, explosion sound, move vertically 8 pps
// coord coord coord (position) 
// short (sprite index)
// byte (scale in 0.1's)
// byte (framerate)
// byte (flags)
//
// The Explosion effect has some flags to control performance/aesthetic features:
#define TE_EXPLFLAG_NONE		0	// all flags clear makes default Half-Life explosion
#define TE_EXPLFLAG_NOADDITIVE	1	// sprite will be drawn opaque (ensure that the sprite you send is a non-additive sprite)
#define TE_EXPLFLAG_NODLIGHTS	2	// do not render dynamic lights
#define TE_EXPLFLAG_NOSOUND		4	// do not play client explosion sound
#define TE_EXPLFLAG_NOPARTICLES	8	// do not draw particles


#define	TE_TAREXPLOSION		4		// Quake1 "tarbaby" explosion with sound
// coord coord coord (position) 

#define	TE_SMOKE			5		// alphablend sprite, move vertically 30 pps
// coord coord coord (position) 
// short (sprite index)
// byte (scale in 0.1's)
// byte (framerate)

#define	TE_TRACER			6		// tracer effect from point to point
// coord, coord, coord (start) 
// coord, coord, coord (end)

#define	TE_LIGHTNING		7		// TE_BEAMPOINTS with simplified parameters
// coord, coord, coord (start) 
// coord, coord, coord (end) 
// byte (life in 0.1's) 
// byte (width in 0.1's) 
// byte (amplitude in 0.01's)
// short (sprite model index)

#define	TE_BEAMENTS			8		
// short (start entity) 
// short (end entity) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define	TE_SPARKS			9		// 8 random tracers with gravity, ricochet sprite
// coord coord coord (position) 

#define	TE_LAVASPLASH		10		// Quake1 lava splash
// coord coord coord (position) 

#define	TE_TELEPORT			11		// Quake1 teleport splash
// coord coord coord (position) 

#define TE_EXPLOSION2		12		// Quake1 colormaped (base palette) particle explosion with sound
// coord coord coord (position) 
// byte (starting color)
// byte (num colors)

#define TE_BSPDECAL			13		// Decal from the .BSP file 
// coord, coord, coord (x,y,z), decal position (center of texture in world)
// short (texture index of precached decal texture name)
// short (entity index)
// [optional - only included if previous short is non-zero (not the world)] short (index of model of above entity)

#define TE_IMPLOSION		14		// tracers moving toward a point
// coord, coord, coord (position)
// byte (radius)
// byte (count)
// byte (life in 0.1's) 

#define TE_SPRITETRAIL		15		// line of moving glow sprites with gravity, fadeout, and collisions
// coord, coord, coord (start) 
// coord, coord, coord (end) 
// short (sprite index)
// byte (count)
// byte (life in 0.1's) 
// byte (scale in 0.1's) 
// byte (velocity along vector in 10's)
// byte (randomness of velocity in 10's)

#define TE_BEAM				16		// obsolete

#define TE_SPRITE			17		// additive sprite, plays 1 cycle
// coord, coord, coord (position) 
// short (sprite index) 
// byte (scale in 0.1's) 
// byte (brightness)

#define TE_BEAMSPRITE		18		// A beam with a sprite at the end
// coord, coord, coord (start position) 
// coord, coord, coord (end position) 
// short (beam sprite index) 
// short (end sprite index) 

#define TE_BEAMTORUS		19		// screen aligned beam ring, expands to max radius over lifetime
// coord coord coord (center position) 
// coord coord coord (axis and radius) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define TE_BEAMDISK			20		// disk that expands to max radius over lifetime
// coord coord coord (center position) 
// coord coord coord (axis and radius) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define TE_BEAMCYLINDER		21		// cylinder that expands to max radius over lifetime
// coord coord coord (center position) 
// coord coord coord (axis and radius) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define TE_BEAMFOLLOW		22		// create a line of decaying beam segments until entity stops moving
// short (entity:attachment to follow)
// short (sprite index)
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte,byte,byte (color)
// byte (brightness)

#define TE_GLOWSPRITE		23		
// coord, coord, coord (pos) short (model index) byte (scale / 10)

#define TE_BEAMRING			24		// connect a beam ring to two entities
// short (start entity) 
// short (end entity) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define TE_STREAK_SPLASH	25		// oriented shower of tracers
// coord coord coord (start position) 
// coord coord coord (direction vector) 
// byte (color)
// short (count)
// short (base speed)
// short (ramdon velocity)

#define TE_BEAMHOSE			26		// obsolete

#define TE_DLIGHT			27		// dynamic light, effect world, minor entity effect
// coord, coord, coord (pos) 
// byte (radius in 10's) 
// byte byte byte (color)
// byte (brightness)
// byte (life in 10's)
// byte (decay rate in 10's)

#define TE_ELIGHT			28		// point entity light, no world effect
// short (entity:attachment to follow)
// coord coord coord (initial position) 
// coord (radius)
// byte byte byte (color)
// byte (life in 0.1's)
// coord (decay rate)

#define TE_TEXTMESSAGE		29
// short 1.2.13 x (-1 = center)
// short 1.2.13 y (-1 = center)
// byte Effect 0 = fade in/fade out
			// 1 is flickery credits
			// 2 is write out (training room)

// 4 bytes r,g,b,a color1	(text color)
// 4 bytes r,g,b,a color2	(effect color)
// ushort 8.8 fadein time
// ushort 8.8  fadeout time
// ushort 8.8 hold time
// optional ushort 8.8 fxtime	(time the highlight lags behing the leading text in effect 2)
// string text message		(512 chars max sz string)
#define TE_LINE				30
// coord, coord, coord		startpos
// coord, coord, coord		endpos
// short life in 0.1 s
// 3 bytes r, g, b

#define TE_BOX				31
// coord, coord, coord		boxmins
// coord, coord, coord		boxmaxs
// short life in 0.1 s
// 3 bytes r, g, b

#define TE_KILLBEAM			99		// kill all beams attached to entity
// short (entity)

#define TE_LARGEFUNNEL		100
// coord coord coord (funnel position)
// short (sprite index) 
// short (flags) 

#define	TE_BLOODSTREAM		101		// particle spray
// coord coord coord (start position)
// coord coord coord (spray vector)
// byte (color)
// byte (speed)

#define	TE_SHOWLINE			102		// line of particles every 5 units, dies in 30 seconds
// coord coord coord (start position)
// coord coord coord (end position)

#define TE_BLOOD			103		// particle spray
// coord coord coord (start position)
// coord coord coord (spray vector)
// byte (color)
// byte (speed)

#define TE_DECAL			104		// Decal applied to a brush entity (not the world)
// coord, coord, coord (x,y,z), decal position (center of texture in world)
// byte (texture index of precached decal texture name)
// short (entity index)

#define TE_FIZZ				105		// create alpha sprites inside of entity, float upwards
// short (entity)
// short (sprite index)
// byte (density)

#define TE_MODEL			106		// create a moving model that bounces and makes a sound when it hits
// coord, coord, coord (position) 
// coord, coord, coord (velocity)
// angle (initial yaw)
// short (model index)
// byte (bounce sound type)
// byte (life in 0.1's)

#define TE_EXPLODEMODEL		107		// spherical shower of models, picks from set
// coord, coord, coord (origin)
// coord (velocity)
// short (model index)
// short (count)
// byte (life in 0.1's)

#define TE_BREAKMODEL		108		// box of models or sprites
// coord, coord, coord (position)
// coord, coord, coord (size)
// coord, coord, coord (velocity)
// byte (random velocity in 10's)
// short (sprite or model index)
// byte (count)
// byte (life in 0.1 secs)
// byte (flags)

#define TE_GUNSHOTDECAL		109		// decal and ricochet sound
// coord, coord, coord (position)
// short (entity index???)
// byte (decal???)

#define TE_SPRITE_SPRAY		110		// spay of alpha sprites
// coord, coord, coord (position)
// coord, coord, coord (velocity)
// short (sprite index)
// byte (count)
// byte (speed)
// byte (noise)

#define TE_ARMOR_RICOCHET	111		// quick spark sprite, client ricochet sound. 
// coord, coord, coord (position)
// byte (scale in 0.1's)

#define TE_PLAYERDECAL		112		// ???
// byte (playerindex)
// coord, coord, coord (position)
// short (entity???)
// byte (decal number???)
// [optional] short (model index???)

#define TE_BUBBLES			113		// create alpha sprites inside of box, float upwards
// coord, coord, coord (min start position)
// coord, coord, coord (max start position)
// coord (float height)
// short (model index)
// byte (count)
// coord (speed)

#define TE_BUBBLETRAIL		114		// create alpha sprites along a line, float upwards
// coord, coord, coord (min start position)
// coord, coord, coord (max start position)
// coord (float height)
// short (model index)
// byte (count)
// coord (speed)

#define TE_BLOODSPRITE		115		// spray of opaque sprite1's that fall, single sprite2 for 1..2 secs (this is a high-priority tent)
// coord, coord, coord (position)
// short (sprite1 index)
// short (sprite2 index)
// byte (color)
// byte (scale)

#define TE_WORLDDECAL		116		// Decal applied to the world brush
// coord, coord, coord (x,y,z), decal position (center of texture in world)
// byte (texture index of precached decal texture name)

#define TE_WORLDDECALHIGH	117		// Decal (with texture index > 256) applied to world brush
// coord, coord, coord (x,y,z), decal position (center of texture in world)
// byte (texture index of precached decal texture name - 256)

#define TE_DECALHIGH		118		// Same as TE_DECAL, but the texture index was greater than 256
// coord, coord, coord (x,y,z), decal position (center of texture in world)
// byte (texture index of precached decal texture name - 256)
// short (entity index)

#define TE_PROJECTILE		119		// Makes a projectile (like a nail) (this is a high-priority tent)
// coord, coord, coord (position)
// coord, coord, coord (velocity)
// short (modelindex)
// byte (life)
// byte (owner)  projectile won't collide with owner (if owner == 0, projectile will hit any client).

#define TE_SPRAY			120		// Throws a shower of sprites or models
// coord, coord, coord (position)
// coord, coord, coord (direction)
// short (modelindex)
// byte (count)
// byte (speed)
// byte (noise)
// byte (rendermode)

#define TE_PLAYERSPRITES	121		// sprites emit from a player's bounding box (ONLY use for players!)
// byte (playernum)
// short (sprite modelindex)
// byte (count)
// byte (variance) (0 = no variance in size) (10 = 10% variance in size)

#define TE_PARTICLEBURST	122		// very similar to lavasplash.
// coord (origin)
// short (radius)
// byte (particle color)
// byte (duration * 10) (will be randomized a bit)

#define TE_FIREFIELD			123		// makes a field of fire.
// coord (origin)
// short (radius) (fire is made in a square around origin. -radius, -radius to radius, radius)
// short (modelindex)
// byte (count)
// byte (flags)
// byte (duration (in seconds) * 10) (will be randomized a bit)
//
// to keep network traffic low, this message has associated flags that fit into a byte:
enum TEFireFlag
{
	/**
	*	All sprites will drift upwards as they animate.
	*/
	TEFIRE_FLAG_ALLFLOAT	= 1,

	/**
	*	Some of the sprites will drift upwards. (50% chance).
	*/
	TEFIRE_FLAG_SOMEFLOAT	= 2,

	/**
	*	If set, sprite plays at 15 fps, otherwise plays at whatever rate stretches the animation over the sprite's duration.
	*/
	TEFIRE_FLAG_LOOP		= 4,

	/**
	*	If set, sprite is rendered alpha blended at 50% else, opaque.
	*/
	TEFIRE_FLAG_ALPHA		= 8,

	/**
	*	If set, all fire sprites have same initial Z instead of randomly filling a cube. 
	*/
	TEFIRE_FLAG_PLANAR		= 16,

	/**
	*	If set, sprite is rendered non-opaque with additive.
	*/
	TEFIRE_FLAG_ADDITIVE	= 32,
};

#define TE_PLAYERATTACHMENT			124 // attaches a TENT to a player (this is a high-priority tent)
// byte (entity index of player)
// coord (vertical offset) ( attachment origin.z = player origin.z + vertical offset )
// short (model index)
// short (life * 10 );

#define TE_KILLPLAYERATTACHMENTS	125 // will expire all TENTS attached to a player.
// byte (entity index of player)

#define TE_MULTIGUNSHOT				126 // much more compact shotgun message
// This message is used to make a client approximate a 'spray' of gunfire.
// Any weapon that fires more than one bullet per frame and fires in a bit of a spread is
// a good candidate for MULTIGUNSHOT use. (shotguns)
//
// NOTE: This effect makes the client do traces for each bullet, these client traces ignore
//		 entities that have studio models.Traces are 4096 long.
//
// coord (origin)
// coord (origin)
// coord (origin)
// coord (direction)
// coord (direction)
// coord (direction)
// coord (x noise * 100)
// coord (y noise * 100)
// byte (count)
// byte (bullethole decal texture index)

#define TE_USERTRACER				127 // larger message than the standard tracer, but allows some customization.
// coord (origin)
// coord (origin)
// coord (origin)
// coord (velocity)
// coord (velocity)
// coord (velocity)
// byte ( life * 10 )
// byte ( color ) this is an index into an array of color vectors in the engine. (0 - )
// byte ( length * 10 )


enum NetMessageType
{
	/**
	*	Unreliable to all
	*/
	MSG_BROADCAST		= 0,

	/**
	*	Reliable to one (msg_entity)
	*/
	MSG_ONE				= 1,

	/**
	*	Reliable to all
	*/
	MSG_ALL				= 2,

	/**
	*	Write to the init string
	*/
	MSG_INIT			= 3,

	/**
	*	Ents in PVS of org
	*/
	MSG_PVS				= 4,

	/**
	*	Ents in PAS of org
	*/
	MSG_PAS				= 5,

	/**
	*	Reliable to PVS
	*/
	MSG_PVS_R			= 6,

	/**
	*	Reliable to PAS
	*/
	MSG_PAS_R			= 7,

	/**
	*	Send to one client, but don't put in reliable stream, put in unreliable datagram ( could be dropped )
	*/
	MSG_ONE_UNRELIABLE	= 8,
	
	/**
	*	Sends to all spectator proxies
	*/
	MSG_SPEC			= 9,
};

/**
*	Contents of a spot in the world
*/
enum Contents
{
	CONTENTS_EMPTY				= -1,
	CONTENTS_SOLID				= -2,
	CONTENTS_WATER				= -3,
	CONTENTS_SLIME				= -4,
	CONTENTS_LAVA				= -5,
	CONTENTS_SKY				= -6,

	//These additional contents constants are defined in bspfile.h
	/**
	*	Removed at CSG time.
	*/
	CONTENTS_ORIGIN				= -7,

	/**
	*	Changed to CONTENTS_SOLID
	*/
	CONTENTS_CLIP				= -8,
	CONTENTS_CURRENT_0			= -9,
	CONTENTS_CURRENT_90			= -10,
	CONTENTS_CURRENT_180		= -11,
	CONTENTS_CURRENT_270		= -12,
	CONTENTS_CURRENT_UP			= -13,
	CONTENTS_CURRENT_DOWN		= -14,
	
	CONTENTS_TRANSLUCENT		= -15,
	CONTENTS_LADDER				= -16,
	
	CONTENT_FLYFIELD			= -17,
	CONTENT_GRAVITY_FLYFIELD	= -18,
	CONTENT_FOG					= -19,
};

/**
*	Sound channels.
*/
enum SoundChannel
{
	CHAN_AUTO				= 0,
	CHAN_WEAPON				= 1,
	CHAN_VOICE				= 2,
	CHAN_ITEM				= 3,
	CHAN_BODY				= 4,

	/**
	*	Allocate stream channel from the static or dynamic area.
	*/
	CHAN_STREAM				= 5,

	/**
	*	Allocate channel from the static area.
	*/
	CHAN_STATIC				= 6,

	/**
	*	Voice data coming across the network.
	*	Network voice data reserves slots (CHAN_NETWORKVOICE_BASE through CHAN_NETWORKVOICE_END).
	*/
	CHAN_NETWORKVOICE_BASE	= 7,

	/**
	*	@copydoc CHAN_NETWORKVOICE_BASE
	*/
	CHAN_NETWORKVOICE_END	= 500,

	/**
	*	Channel used for bot chatter.
	*/
	CHAN_BOT				= 501,
};

// attenuation values
#define ATTN_NONE		0
#define	ATTN_NORM		(float)0.8
#define ATTN_IDLE		(float)2
#define ATTN_STATIC		(float)1.25 

/**
*	Pitch values.
*	100 is non-pitch shifted. Other values are possible - 0-255, where 255 is very high.
*/
enum Pitch
{
	PITCH_NORM		= 100,
	PITCH_LOW		= 95,
	PITCH_HIGH		= 120,
};

/**
*	Volume values.
*/
#define VOL_NORM		1.0

// buttons
#include "in_buttons.h"

/**
*	Break Model Defines.
*/
enum BreakModel
{
	BREAK_TYPEMASK	= 0x4F,
	BREAK_GLASS		= 0x01,
	BREAK_METAL		= 0x02,
	BREAK_FLESH		= 0x04,
	BREAK_WOOD		= 0x08,
	
	BREAK_SMOKE		= 0x10,
	BREAK_TRANS		= 0x20,
	BREAK_CONCRETE	= 0x40,
	BREAK_2			= 0x80,
};

/**
*	Colliding temp entity sounds.
*/
enum BounceSound
{
	BOUNCE_GLASS		= BREAK_GLASS,
	BOUNCE_METAL		= BREAK_METAL,
	BOUNCE_FLESH		= BREAK_FLESH,
	BOUNCE_WOOD			= BREAK_WOOD,
	BOUNCE_SHRAP		= 0x10,
	BOUNCE_SHELL		= 0x20,
	BOUNCE_CONCRETE		= BREAK_CONCRETE,
	BOUNCE_SHOTSHELL	= 0x80
};

/**
*	Temp entity bounce sound types.
*/
enum TE_Bounce
{
	TE_BOUNCE_NULL		= 0,
	TE_BOUNCE_SHELL		= 1,
	TE_BOUNCE_SHOTSHELL	= 2,
};

/**
*	Rendering constants.
*/
enum RenderMode
{
	/**
	*	src
	*/
	kRenderNormal,

	/**
	*	c*a+dest*(1-a)
	*/
	kRenderTransColor,

	/**
	*	src*a+dest*(1-a)
	*/
	kRenderTransTexture,

	/**
	*	src*a+dest -- No Z buffer checks
	*/
	kRenderGlow,

	/**
	*	src*srca+dest*(1-srca)
	*/
	kRenderTransAlpha,

	/**
	*	src*a+dest
	*/
	kRenderTransAdd,
};

enum RenderFX
{	
	kRenderFxNone = 0, 
	kRenderFxPulseSlow, 
	kRenderFxPulseFast, 
	kRenderFxPulseSlowWide, 
	kRenderFxPulseFastWide, 
	kRenderFxFadeSlow, 
	kRenderFxFadeFast, 
	kRenderFxSolidSlow, 
	kRenderFxSolidFast, 	   
	kRenderFxStrobeSlow, 
	kRenderFxStrobeFast, 
	kRenderFxStrobeFaster, 
	kRenderFxFlickerSlow, 
	kRenderFxFlickerFast,
	kRenderFxNoDissipation,

	/**
	*	Distort/scale/translate flicker.
	*/
	kRenderFxDistort,

	/**
	*	kRenderFxDistort + distance fade.
	*/
	kRenderFxHologram,

	/**
	*	kRenderAmt is the player index.
	*/
	kRenderFxDeadPlayer,

	/**
	*	Scale up really big!
	*/
	kRenderFxExplode,

	/**
	*	Glowing Shell.
	*/
	kRenderFxGlowShell,

	/**
	*	Keep this sprite from getting very small (SPRITES only!)
	*/
	kRenderFxClampMinScale,

	/**
	*	CTM !!!CZERO added to tell the studiorender that the value in iuser2 is a lightmultiplier.
	*/
	kRenderFxLightMultiplier,
};


typedef unsigned int	func_t;
typedef unsigned int	string_t;

typedef unsigned char 		byte;
typedef unsigned short 		word;

//Deal with any legacy code that messes with stuff. - Solokiller
#if defined( true ) || defined( false )
#error "Don't define true and false!"
#endif

typedef int qboolean;

typedef struct
{
	byte r, g, b;
} color24;

typedef struct
{
	unsigned r, g, b, a;
} colorVec;

#ifdef _WIN32
#pragma pack(push,2)
#endif

typedef struct
{
	unsigned short r, g, b, a;
} PackedColorVec;

#ifdef _WIN32
#pragma pack(pop)
#endif
typedef struct link_s
{
	struct link_s	*prev, *next;
} link_t;

typedef struct edict_s edict_t;

typedef struct
{
	Vector	normal;
	float	dist;
} plane_t;

typedef struct
{
	qboolean	allsolid;	// if true, plane is not valid
	qboolean	startsolid;	// if true, the initial point was in a solid area
	qboolean	inopen, inwater;
	float	fraction;		// time completed, 1.0 = didn't hit anything
	Vector	endpos;			// final position
	plane_t	plane;			// surface normal at impact
	edict_t	*ent;			// entity the surface is on
	int		hitgroup;		// 0 == generic, non zero is specific body part
} trace_t;

namespace Hull
{
/**
*	Hull types.
*/
enum Hull
{
	/**
	*	Point hull.
	*/
	POINT	= 0,

	/**
	*	Human hull. Used by players when standing.
	*/
	HUMAN	= 1,

	/**
	*	Large hull.
	*/
	LARGE	= 2,

	/**
	*	Head hull. Used by players when crouching.
	*/
	HEAD	= 3,

	/**
	*	Number of hulls. Not a valid hull number.
	*/
	COUNT
};
}

enum ALERT_TYPE
{
	at_notice,

	/**
	*	Same as at_notice, but forces a ConPrintf, not a message box.
	*/
	at_console,

	/**
	*	Same as at_console, but only shown if developer level is 2!
	*/
	at_aiconsole,
	at_warning,
	at_error,

	/**
	*	Server print to console ( only in multiplayer games ).
	*/
	at_logged
};

#define BSP_FILE_EXT ".bsp"

/**
*	Engine limits.
*/
enum EngineLimit
{
	/**
	*	Maximum number of models that can be precached.
	*/
	MAX_MODELS		= 512,

	/**
	*	Maximum number of sounds that can be precached.
	*/
	MAX_SOUNDS		= 512,

	/**
	*	Maximum number of generic files that can be precached.
	*/
	MAX_GENERIC		= 512,

	/**
	*	Maximum number of events that can be precached.
	*/
	MAX_EVENTS		= 256,
};

#endif

