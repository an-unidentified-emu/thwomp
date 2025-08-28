// thwomp.inc.c

void grindel_thwomp_act_on_ground(void) {
    if (o->oTimer == 0) {
        o->oThwompRandomTimer = random_float() * 10.0f + 20.0f;
    }
    if (o->oTimer > o->oThwompRandomTimer) {
        o->oAction = GRINDEL_THWOMP_ACT_RISING;
    }
}

void grindel_thwomp_act_falling(void) {
    o->oVelY += -4.0f;
    o->oPosY += o->oVelY;
    if (o->oPosY < o->oHomeY) {
        o->oPosY = o->oHomeY;
        o->oVelY = 0.0f;
        o->oAction = GRINDEL_THWOMP_ACT_LAND;
    }
}

void grindel_thwomp_act_land(void) {
    if (o->oTimer == 0 && o->oDistanceToMario < 1500.0f) {
        cur_obj_shake_screen(SHAKE_POS_SMALL);
        cur_obj_play_sound_2(SOUND_OBJ_THWOMP);
    }
    if (o->oTimer >= 10) {
        o->oAction = GRINDEL_THWOMP_ACT_ON_GROUND;
    }
}

void grindel_thwomp_act_floating(void) {
    if (o->oTimer == 0) {
        o->oThwompRandomTimer = random_float() * 30.0f + 10.0f;
    }
    if (o->oTimer > o->oThwompRandomTimer) {
        o->oAction = GRINDEL_THWOMP_ACT_FALLING;
    }
}

void grindel_thwomp_act_rising(void) {
    if (o->oBehParams2ndByte + 40 < o->oTimer) {
        o->oAction = GRINDEL_THWOMP_ACT_FLOATING;
        o->oPosY += 5.0f;
    } else {
        o->oPosY += 10.0f;
    }
}

ObjActionFunc sGrindelThwompActions[] = {
    grindel_thwomp_act_rising,
    grindel_thwomp_act_floating,
    grindel_thwomp_act_falling,
    grindel_thwomp_act_land,
    grindel_thwomp_act_on_ground
};

void bhv_grindel_thwomp_loop(void) {
    cur_obj_call_action_function(sGrindelThwompActions);
}


/********************************************
 *********    NEW THWOMPS   *****************
 ********************************************
*/
// For animations
#define THWOMP_FACE_NORMAL  0
#define THWOMP_FACE_CLOSED  1
#define THWOMP_FACE_PEEKING 2
#define THWOMP_WAIT 0
#define THWOMP_ATTACK 1
#define THWOMP_AT_BOTTOM 2
#define THWOMP_RISE 3

void bhv_new_thwomp_init(void) {
    o->oFlipDirection = -1;
    if (GET_BPARAM1(o->oBehParams) == 0x01) { //flip attack direction
        o->oFlipDirection = 1;
    }

    Vec3f orig = { o->oPosX, o->oPosY, o->oPosZ };
    Vec3f dir  = { 0.0f, 3000.0f*o->oFlipDirection, 0.0f}; // cast a ray 3000 units along -Y
    struct Surface *hit_surface;
    Vec3f hit_pos;
    
    find_surface_on_ray(orig, dir, &hit_surface, hit_pos, RAYCAST_FIND_FLOOR);
    o->oThwompFaceState = THWOMP_FACE_CLOSED;
    o->oHitPosX = hit_pos[0];
    o->oHitPosY = hit_pos[1];
    o->oHitPosZ = hit_pos[2];
}
void thwomp_act_wait(void) {
    if (o->oTimer > 30) {
        // Relative Y distance (signed)
        f32 dy = (gMarioObject->oPosY - o->oPosY) * o->oFlipDirection;

    // --- Peeking face activation ---
        if (dy >= 100 && dy <= 1000 &&   // Mario is 100–1000 units in front
            gMarioObject->oPosX <= o->oPosX + 400 &&
            gMarioObject->oPosX >= o->oPosX - 400 &&
            gMarioObject->oPosZ <= o->oPosZ + 500 &&
            gMarioObject->oPosZ >= o->oPosZ - 400) {

            // --- Movement activation ---
            if (dy >= 100 && dy <= 1000 &&   // same Y window but tighter X range
                gMarioObject->oPosX <= o->oPosX + 200 &&
                gMarioObject->oPosX >= o->oPosX - 200 &&
                gMarioObject->oPosZ <= o->oPosZ + 500 &&
                gMarioObject->oPosZ >= o->oPosZ - 200) {

                o->oAction = THWOMP_ATTACK;
                o->oTimer = 0;
            }
        }

        o->oThwompFaceState = THWOMP_FACE_CLOSED;
}
}

void thwomp_act_attack(void) {
// Target stop position: 255 units away from the wall, depending on direction
    f32 targetY = o->oHitPosY - 1.0f * o->oFlipDirection;

    // Check if we've reached/passed the target in the direction we're moving
    if ((o->oPosY - targetY) * o->oFlipDirection >= 0.0f) {
        o->oVelY = 0.0f;
        o->oTimer = 0;
        o->oPosY = targetY;
        o->oAction = THWOMP_AT_BOTTOM;
        if (o->oTimer == 0 && o->oDistanceToMario < 1500.0f) {
            cur_obj_shake_screen(SHAKE_POS_SMALL);
            cur_obj_play_sound_2(SOUND_OBJ_THWOMP);
        }
    }

    o->oVelY += 10.0f * o->oFlipDirection;
    if (o->oVelZ > 75.0f) o->oVelZ = 75.0f;
    o->oPosY += o->oVelY;
}

void thwomp_act_at_bottom(void) {
    if (o->oTimer > 30)
    o->oAction = THWOMP_RISE;
}

void thwomp_act_rise(void) {
if (o->oPosY * -o->oFlipDirection >= o->oHomeY) {
        o->oTimer = 0;
        o->oAction = THWOMP_WAIT;
    } else
    o->oPosY -= 10.0f * o->oFlipDirection;
}

ObjActionFunc sNewThwompActions[] = {
    thwomp_act_wait,
    thwomp_act_attack,
    thwomp_act_at_bottom,
    thwomp_act_rise

};

void bhv_new_thwomp_loop(void){
    cur_obj_call_action_function(sNewThwompActions);
    o->oThwompFaceState = THWOMP_FACE_CLOSED;
}


/********************************************
***********     SIDEWAYS THWOMPS ************
*********************************************
Generates 2 raycast boxes, the bigger one for the peeking
and the smaller for activation
*/

void bhv_sideways_thwomp_init(void){
    o->oFlipDirection = 1;
    if (GET_BPARAM1(o->oBehParams) == 0x01) { //flip attack direction
        o->oFlipDirection = -1;
    }

    Vec3f orig = { o->oPosX, o->oPosY, o->oPosZ };
    Vec3f dir  = { 0.0f, 0.0f, 3000.0f*o->oFlipDirection }; // cast a ray 3000 units along +Z
    struct Surface *hit_surface;
    Vec3f hit_pos;
    
    find_surface_on_ray(orig, dir, &hit_surface, hit_pos, RAYCAST_FIND_WALL);
    o->oThwompFaceState = THWOMP_FACE_CLOSED;
    o->oHitPosX = hit_pos[0];
    o->oHitPosY = hit_pos[1];
    o->oHitPosZ = hit_pos[2];
}

#define SIDEWAYS_THWOMP_WAIT 0
#define SIDEWAYS_THWOMP_ATTACK 1
#define SIDEWAYS_THWOMP_AT_END 2
#define SIDEWAYS_THWOMP_RETURN 3
void sideways_thwomp_wait(void) {
    // Relative Z distance (signed)
    f32 dz = (gMarioObject->oPosZ - o->oPosZ) * o->oFlipDirection;

    // --- Peeking face activation ---
    if (dz >= 100 && dz <= 1000 &&   // Mario is 100–1000 units in front
        gMarioObject->oPosX <= o->oPosX + 400 &&
        gMarioObject->oPosX >= o->oPosX - 400 &&
        gMarioObject->oPosY <= o->oPosY + 500 &&
        gMarioObject->oPosY >= o->oPosY - 400) {

        // --- Movement activation ---
        if (dz >= 100 && dz <= 1000 &&   // same Z window but tighter X range
            gMarioObject->oPosX <= o->oPosX + 200 &&
            gMarioObject->oPosX >= o->oPosX - 200 &&
            gMarioObject->oPosY <= o->oPosY + 500 &&
            gMarioObject->oPosY >= o->oPosY - 200) {

            o->oAction = SIDEWAYS_THWOMP_ATTACK;
            o->oTimer = 0;
        }
    }

    o->oThwompFaceState = THWOMP_FACE_CLOSED;
}


void sideways_thwomp_attack(void) {
    // Target stop position: 255 units away from the wall, depending on direction
    f32 targetZ = o->oHitPosZ - 255.0f * o->oFlipDirection;

    // Check if we've reached/passed the target in the direction we're moving
    if ((o->oPosZ - targetZ) * o->oFlipDirection >= 0.0f) {
        o->oVelZ = 0.0f;
        o->oTimer = 0;
        o->oPosZ = targetZ;
        o->oAction = SIDEWAYS_THWOMP_AT_END;
    }

    o->oVelZ += 10.0f * o->oFlipDirection;
    if (o->oVelZ > 75.0f) o->oVelZ = 75.0f;
    o->oPosZ += o->oVelZ;
}

void sideways_thwomp_at_end(void){
    if (o->oTimer > 20)
    o->oAction = SIDEWAYS_THWOMP_RETURN;
}
void sideways_thwomp_return(void){
    if (o->oPosZ * -o->oFlipDirection >= o->oHomeZ) {
        //o->oPosZ = o->oHomeZ;
        o->oAction = SIDEWAYS_THWOMP_WAIT;
    } else
    o->oPosZ -= 10.0f * o->oFlipDirection;

}

ObjActionFunc sSidewaysThwompActions[] = {
    sideways_thwomp_wait,
    sideways_thwomp_attack,
    sideways_thwomp_at_end,
    sideways_thwomp_return
};

void bhv_sideways_thwomp_loop(void) {
    cur_obj_call_action_function(sSidewaysThwompActions);
    o->oThwompFaceState = THWOMP_FACE_CLOSED;
}




/********************************************
 *********    THWIMPS   *********************
 ********************************************

bhvParam 1 does nothing
bhvParam 2 is for the range in both directions on the X axis
bhvParam 3 is some multiple of height, very volatile
*/

static struct ObjectHitbox sThwimpHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 80,
    /* hurtboxRadius:     */ 80,
    /* hurtboxHeight:     */ 80,
};

#define THWIMP_ACT_ON_GROUND 0
#define THWIMP_ACT_JUMP 1
#define X_AXIS_THWIMP 0
#define Z_AXIS_THWIMP 1
#define OCTAGONAL_THWIMP 2

void bhv_thwimp_init(void) {
    o->oThwimpCycle = 1;
    if (GET_BPARAM1(o->oBehParams) == 0x01) {
        o->oMoveAngleYaw += DEGREES(90);
        o->oThwimpType = Z_AXIS_THWIMP;
        o->oTimer = 20;
    } else if (GET_BPARAM1(o->oBehParams) == 0x02) o->oThwimpType = OCTAGONAL_THWIMP;
    else o->oThwimpType = X_AXIS_THWIMP;

    o->oThwimpXRange = GET_BPARAM2(o->oBehParams);
    o->oThwimpYRange = GET_BPARAM3(o->oBehParams);
}
void thwimp_act_on_ground(void) {
    if (o->oTimer > 20) {
        o->oAction = THWIMP_ACT_JUMP;
        o->oTimer = 0;
        if (o->oThwimpCycle > 1) o->oThwimpCycle = 1;
        else o->oThwimpCycle = 2;
    }
}

void thwimp_act_jump(void) {
    if (o->oTimer == 0) o->oThwompRandomTimer = random_u16() %5;
    s32 x = (1.5*o->oTimer - 20);
    o->oPosY = (-1*o->oThwimpYRange)*(x - 20) * (x + 20) + o->oHomeY;

    if (o->oThwompRandomTimer == 0){ //randomly rotates 1 in 5 times
        if (o->oTimer >= 2 && o->oTimer <= 25) {
            if (o->oThwimpCycle == 1)
            o->oFaceAngleRoll -= DEGREES(15);
            if (o->oThwimpCycle == 2)
            o->oFaceAngleRoll += DEGREES(15);
        }
    }

    if (o->oPosY < o->oHomeY){
            o->oPosY = o->oHomeY;
            o->oTimer = 0;
    cur_obj_play_sound_2(SOUND_OBJ_THWOMP);
         o->oAction = THWIMP_ACT_ON_GROUND;
    }
    else {
        if (o->oThwimpType == X_AXIS_THWIMP){
            if (o->oThwimpCycle == 1) o->oPosX+=o->oThwimpXRange;
            else o->oPosX-=o->oThwimpXRange;
         } else if (o->oThwimpType == Z_AXIS_THWIMP) {
            if (o->oThwimpCycle == 1) o->oPosZ+=o->oThwimpXRange;
            else o->oPosZ-=o->oThwimpXRange;
         }
    }

}
ObjActionFunc sThwimpActions[] = {
    thwimp_act_on_ground,
    thwimp_act_jump
};

void bhv_thwimp_loop(void) {
    obj_set_hitbox(o, &sThwimpHitbox);
    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        o->oInteractStatus = 0;
    }
    cur_obj_call_action_function(sThwimpActions);
}

/*Gfx *geo_new_thwomp_eyes(s32 callContext, struct GraphNode *node) {
    if (callContext != GEO_CONTEXT_RENDER) {
        return NULL;
    }

    struct Object *obj = (struct Object *)gCurGraphNodeObject; 
    if (obj == NULL) {
        return NULL;
    }

    // Give plenty of space (safe upper bound ~20 commands)
    Gfx *dl = alloc_display_list(20 * sizeof(Gfx));
    Gfx *head = dl;

    // --- Material setup (copied from your mat_new_thwomp_face) ---
    gSPTexture(head++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineLERP(head++,
        TEXEL0, 0, SHADE, 0,   0, 0, 0, ENVIRONMENT,
        TEXEL0, 0, SHADE, 0,   0, 0, 0, ENVIRONMENT);
    gDPSetTextureFilter(head++, G_TF_BILERP);
    gDPSetRenderMode(head++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    // Tell RDP we're using a palette
    gDPSetTextureLUT(head++, G_TT_RGBA16);
    o->oThwompFaceState = 1;
    switch (o->oThwompFaceState) {
        case 0: // open eyes
            gDPLoadTLUT_pal16(head++, 0, new_thwomp_thwomp_face_grey_rgba16_pal_rgba16);
            gDPLoadTextureBlock(head++, new_thwomp_thwomp_face_grey_rgba16_ci8,
                G_IM_FMT_CI, G_IM_SIZ_8b,
                32, 32, 0,
                G_TX_WRAP, G_TX_WRAP,
                5, 5,
                G_TX_NOLOD, G_TX_NOLOD);
            break;

        case 1: // closed eyes
            gDPLoadTLUT_pal16(head++, 0, new_thwomp_thwomp_face_grey_closed_eyes_rgba16_pal_rgba16);
            gDPLoadTextureBlock(head++, new_thwomp_thwomp_face_grey_closed_eyes_rgba16_ci8,
                G_IM_FMT_CI, G_IM_SIZ_8b,
                32, 32, 0,
                G_TX_WRAP, G_TX_WRAP,
                5, 5,
                G_TX_NOLOD, G_TX_NOLOD);
            break;

        case 2: // peeking
            gDPLoadTLUT_pal16(head++, 0, new_thwomp_thwomp_face_grey_peek_rgba16_pal_rgba16);
            gDPLoadTextureBlock(head++, new_thwomp_thwomp_face_grey_peek_rgba16_ci8,
                G_IM_FMT_CI, G_IM_SIZ_8b,
                32, 32, 0,
                G_TX_WRAP, G_TX_WRAP,
                5, 5,
                G_TX_NOLOD, G_TX_NOLOD);
            break;
    }

    gSPEndDisplayList(head);
    print_text_fmt_int(0,0,"%d", o->oThwompFaceState);
    return dl;
}
*/