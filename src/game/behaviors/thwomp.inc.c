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
***********     SIDEWAYS THWOMPS ************
*********************************************
Generates 2 raycast boxes, the bigger one for the peeking
and the smaller for activation
*/
UNUSED bhv_sideways_thwomp_init(void){
    Vec3f orig = {o->oPosX, o->oPosY, o->oPosZ};
    Vec3f dir = {0,0,0};
    Vec3f hit_pos[2];
    struct Surface *wall;

    find_surface_on_ray(orig, dir, &wall, hit_pos, RAYCAST_FIND_FLOOR | RAYCAST_FIND_CEIL | RAYCAST_FIND_WALL);
    //if (hit_pos != NULL){
    o->oPosX = (s32)hit_pos[0];
    //o->oHomeX = o->oPosX;
    //}
}

#define SIDEWAYS_THWOMP_WAIT 0
#define SIDEWAYS_THWOMP_ATTACK 1
#define SIDEWAYS_THWOMP_AT_END 2
#define SIDEWAYS_THWOMP_RETURN 3
void sideways_thwomp_wait(void) {
    if (gMarioObject->oPosX >= o->oPosX - 1000 && // activates the peeking face
        gMarioObject->oPosX <= o->oPosX - 100  &&
        gMarioObject->oPosZ <= o->oPosZ + 400  &&
        gMarioObject->oPosZ >= o->oPosZ - 400  &&
        gMarioObject->oPosY <= o->oPosY + 500  &&
        gMarioObject->oPosY >= o->oPosY - 400 ) {

        
    if (gMarioObject->oPosX >= o->oPosX - 1000 && // activates the movement
        gMarioObject->oPosX <= o->oPosX - 100  &&
        gMarioObject->oPosZ <= o->oPosZ + 200  &&
        gMarioObject->oPosZ >= o->oPosZ - 200  &&
        gMarioObject->oPosY <= o->oPosY + 500  &&
        gMarioObject->oPosY >= o->oPosY - 200 ) {
        o->oAction = SIDEWAYS_THWOMP_ATTACK;
        o->oTimer = 0; }
        }
}

void sideways_thwomp_attack(void) {
    if (o->oTimer > 50) {
        o->oVelX = 0.0f;
        o->oTimer = 0;
        o->oAction = SIDEWAYS_THWOMP_AT_END;
    }
    o->oVelX += -50.0f;
    o->oPosX += o->oVelX;
 
}

void sideways_thwomp_at_end(void){
    if (o->oTimer > 20)
    o->oAction = SIDEWAYS_THWOMP_RETURN;
}
void sideways_thwomp_return(void){
    if (o->oPosX >= o->oHomeX) {
        o->oPosX = o->oHomeX;
        o->oAction = SIDEWAYS_THWOMP_WAIT;
    } else 
    o->oPosX += 5.0f;
    
}

ObjActionFunc sSidewaysThwompActions[] = {
    sideways_thwomp_wait,
    sideways_thwomp_attack,
    sideways_thwomp_at_end,
    sideways_thwomp_return
};

void bhv_sideways_thwomp_loop(void) {
    cur_obj_update_floor_and_walls();
    //print_text_fmt_int(0,0, "%d", o->oAction);
    cur_obj_move_standard(78);
    cur_obj_call_action_function(sSidewaysThwompActions);
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

/********************************************
 *********    NEW THWOMPS   *****************
 ********************************************
*/
void thwomp_act_wait(void) {

}

void thwomp_act_attack(void) {

}

void thwomp_act_at_bottom(void) {

}

void thwomp_act_rise(void) {

}

ObjActionFunc sNewThwompActions[] = {
    thwomp_act_wait,
    thwomp_act_attack,
    thwomp_act_at_bottom,
    thwomp_act_rise

};

void bhv_new_thwomp_loop(void){
    cur_obj_call_action_function(sNewThwompActions);
}