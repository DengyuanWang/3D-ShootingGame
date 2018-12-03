//
//  Monster.cpp
//  3D_shooting_adventure
//
//  Created by 王登远 on 11/28/18.
//  Copyright © 2018 Dengyuan Wang. All rights reserved.
//

#include "Monster.hpp"
#include "../GameLogic/Game_Obj.hpp"
Monster::Monster(){
    Death_tag = false;//default as false
    Health = 5;//0~100
    Speed = 0;//default as 1
    //vector<string> Abilities;//default as "walk"
}
Static_M::Static_M(){
    
}
void Static_M::Update(UI_Event &UIEvent, void *ptr_in){
    Game_Events* Gptr =(Game_Events*)Gevent_list;//get game event handler
    Game_Obj* ptr =(Game_Obj*)ptr_in;
    vector<Game_Obj> *Gobj_list_ptr;
    Gobj_list_ptr =(vector<Game_Obj> *)Gobj_list;
    for(int i=0;i<Gobj_list_ptr->size();i++)
    {
        //cout<<"monster:"<<ptr->Index<<"others"<<(*Gobj_list_ptr)[i].Index<<endl;
        if(((*Gobj_list_ptr)[i].Index)!=ptr->Index//not self
           &&ptr->check_collision(&(*Gobj_list_ptr)[i]))//collide with others
        {
            if((*Gobj_list_ptr)[i].get_type()=="bullet")//collide with bullet
            {
                Bullet *tmpptr = (Bullet*)(*Gobj_list_ptr)[i].Comp_list[0];
                string b=tmpptr->ownership;
                if(b=="player")//not self bullet
                {
                    
                        //Gptr->Erase_index.push_back(i);//erase bullet
                        Health--;
                        if(Health<=0){
                            Gptr->set_event("Erase", true);
                            Gptr->Erase_index.push_back(ptr->Index);//kill self
                            return;
                        }
                }
            }
        }
    }
}
