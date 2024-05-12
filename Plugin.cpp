// This file added in headers queue
// File: "Sources.h"
#include "resource.h"



namespace GOTHIC_ENGINE {
    
    //0x0073A490 public: void __thiscall oCNpc::EquipArmor(class oCItem *)
    HOOK Ivk_oCNpc_EquipArmor PATCH(&oCNpc::EquipArmor, &oCNpc::EquipArmor_Union);
   
    void oCNpc::EquipArmor_Union(oCItem* item) {
        if (this == player) {
            if (OPTION_ARMOR_CHANGED == 1) {
                zSTRING x = OPTION_ARMOR_NAME;
                item->visual_change = x;
            }

            THISCALL(Ivk_oCNpc_EquipArmor)(item);
        }
        else {
            THISCALL(Ivk_oCNpc_EquipArmor)(item);
        }
    }
    Menu menu;
    Array<oCItem*> armors;


    zCMenuItemList* menuItemList = new zCMenuItemList();

    zCMenu* armorMenu = new zCMenu();


    void Game_Loop() {
        if (zKeyToggled(GetEmulationKeyCode(OPTION_OPEN_BUTTON))) {
            if (player->GetEquippedArmor()) {
                if (armorMenu->m_screenInitDone == 1) {
                    armorMenu->Leave();
                    menuItemList->SetIsActive(0);
                    Gothic::Game::Session->Unpause();
                }
                else {
                    armorMenu->Enter();
                    menuItemList->JumpToTopEntry();
                    menuItemList->DrawFront();
                    menuItemList->SetIsActive(1);
                    Gothic::Game::Session->Pause();
                }
            }
            else {
                ogame->GetTextView()->Printwin("YOU NEED TO WEAR ARMOR!");
            }
        }

        if (armorMenu->m_screenInitDone == 1) {
            if (zKeyToggled(KEY_UPARROW)) {
                menuItemList->JumpToPrevEntry();
            }
            if (zKeyToggled(KEY_DOWNARROW)) {
                menuItemList->JumpToNextEntry();
            }
            if (zKeyToggled(KEY_SPACE)) {
                if (OPTION_ARMOR_NUMBER == 0) {
                    menuItemList->m_list.GetSafe(OPTION_ARMOR_NUMBER).title = "CLEAR";

                }
                else {
                    menuItemList->m_list.GetSafe(OPTION_ARMOR_NUMBER).title = armors[OPTION_ARMOR_NUMBER - 1]->name;
                }
                zoptions->WriteInt(PLUGIN_NAME, "ArmorChangedNumber", menuItemList->m_cur_element, 0);
                menuItemList->m_list.GetSafe(menuItemList->m_cur_element).title += " <- CURRENT ELEMENT";
                armorMenu->RedrawItems();

                if (menuItemList->m_cur_element == 0) {
                    zoptions->WriteInt(PLUGIN_NAME, "ArmorChanged", 0, 0);
                    zoptions->WriteString(PLUGIN_NAME, "ArmorName", "", 0);
                    ogame->GetTextView()->Printwin("YOU NEED TO RESTART RELOAD SAVE!");

                }
                else {
                    zoptions->WriteInt(PLUGIN_NAME, "ArmorChanged", 1, 0);
                    zoptions->WriteString(PLUGIN_NAME, "ArmorName", menuItemList->m_list.GetSafe(menuItemList->m_cur_element).content, 0);
                    oCItem* armor = player->GetEquippedArmor();
                    armor->visual_change = menuItemList->m_list.GetSafe(menuItemList->m_cur_element).content;
                    player->UnequipItem(armor);
                    player->EquipArmor(armor);
                }

            }
        }
    }
    void LoadEnd() {
        if (OPTION_ARMOR_CHANGED == 1) {
            if (player->GetEquippedArmor()) {
                zSTRING x = OPTION_ARMOR_NAME;
                player->GetEquippedArmor()->visual_change = x;
                oCItem* armor = player->GetEquippedArmor();
                player->UnequipItem(armor);
                player->EquipArmor(armor);
            }
            
        }
       
        zCFont* fn = new zCFont("Font_Old_10_White.tga");
        zCFont* fnHi = new zCFont("Font_Old_10_White_Hi.tga");

        armorMenu->posx = 100;
        armorMenu->posy = 100;
        armorMenu->dimx = 4096;
        armorMenu->dimy = 7500;
        armorMenu->m_use_posx = 150;
        armorMenu->m_use_posy = 150;
        armorMenu->m_use_dimx = 4096;
        armorMenu->m_use_dimy = 7500;
        armorMenu->backPic = "Menu_Ingame.tga";


        menuItemList->zCMenuItemList_OnInit("AHA32");
        menuItemList->SetPos(300, 300);
        menuItemList->SetSize(7902, 7902);
        menuItemList->m_pFont = fn;
        menuItemList->m_pFontHi = fnHi;
        menuItemList->m_parPosX = 200;
        menuItemList->m_parPosY = 200;
        menuItemList->m_parDimX = 7702;
        menuItemList->m_parDimY = 7702;
        armorMenu->m_listItems.Insert(menuItemList);

        zTMenuItemListElement* firstItemListElem = new zTMenuItemListElement();
        firstItemListElem->title = "CLEAR";
        firstItemListElem->content = "REMOVE ARMOR LOOK";
        menuItemList->InsertEnd(*firstItemListElem);

        for (unsigned int i = 0; i < armors.GetNum(); i++) {
            zTMenuItemListElement* itemListElem = new zTMenuItemListElement();
            itemListElem->title = armors[i]->name;
            itemListElem->content = armors[i]->visual_change;
            menuItemList->InsertEnd(*itemListElem);
        }

        menuItemList->m_list.GetSafe(OPTION_ARMOR_NUMBER).title += " <- CURRENT ELEMENT";

        armorMenu->ScreenInit();
        armorMenu->ScreenDone();

        // MINIMUM DO WYŚWIETLENIA cMenuItem
        //text->SetPos(500, 500);
        //text->SetFont("Font_Old_10_White.tga");
        //text->SetText("texttexttext|texttexttexttexttexttexttext ", 0, 0);
        
        
    }

  void Game_Entry() {
  }
  
  void Game_Init() {
      armors = menu.GetArmors();
  }

  void Game_Exit() {
  }

  void Game_PreLoop() {
  }
 

  void Game_PostLoop() {
  }

  void Game_MenuLoop() {
  }

  // Information about current saving or loading world
  TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

  void Game_SaveBegin() {
  }

  void Game_SaveEnd() {
  }


  void LoadBegin() {

  }



  void Game_LoadBegin_NewGame() {
    LoadBegin();
  }

  void Game_LoadEnd_NewGame() {
    LoadEnd();
  }

  void Game_LoadBegin_SaveGame() {
    LoadBegin();
  }

  void Game_LoadEnd_SaveGame() {
    LoadEnd();
  }

  void Game_LoadBegin_ChangeLevel() {
    LoadBegin();
  }

  void Game_LoadEnd_ChangeLevel() {
    LoadEnd();
  }

  void Game_LoadBegin_Trigger() {
  }
  
  void Game_LoadEnd_Trigger() {
  }
  
  void Game_Pause() {
  }
  
  void Game_Unpause() {
  }
  
  void Game_DefineExternals() {
  }

  void Game_ApplyOptions() {
  }

  /*
  Functions call order on Game initialization:
    - Game_Entry           * Gothic entry point
    - Game_DefineExternals * Define external script functions
    - Game_Init            * After DAT files init
  
  Functions call order on Change level:
    - Game_LoadBegin_Trigger     * Entry in trigger
    - Game_LoadEnd_Trigger       *
    - Game_Loop                  * Frame call window
    - Game_LoadBegin_ChangeLevel * Load begin
    - Game_SaveBegin             * Save previous level information
    - Game_SaveEnd               *
    - Game_LoadEnd_ChangeLevel   *
  
  Functions call order on Save game:
    - Game_Pause     * Open menu
    - Game_Unpause   * Click on save
    - Game_Loop      * Frame call window
    - Game_SaveBegin * Save begin
    - Game_SaveEnd   *
  
  Functions call order on Load game:
    - Game_Pause              * Open menu
    - Game_Unpause            * Click on load
    - Game_LoadBegin_SaveGame * Load begin
    - Game_LoadEnd_SaveGame   *
  */

#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( AppDefault ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( AppDefault ) Game_Exit,
    Enabled( AppDefault ) Game_PreLoop,
    Enabled( AppDefault ) Game_Loop,
    Enabled( AppDefault ) Game_PostLoop,
    Enabled( AppDefault ) Game_MenuLoop,
    Enabled( AppDefault ) Game_SaveBegin,
    Enabled( AppDefault ) Game_SaveEnd,
    Enabled( AppDefault ) Game_LoadBegin_NewGame,
    Enabled( AppDefault ) Game_LoadEnd_NewGame,
    Enabled( AppDefault ) Game_LoadBegin_SaveGame,
    Enabled( AppDefault ) Game_LoadEnd_SaveGame,
    Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
    Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
    Enabled( AppDefault ) Game_LoadBegin_Trigger,
    Enabled( AppDefault ) Game_LoadEnd_Trigger,
    Enabled( AppDefault ) Game_Pause,
    Enabled( AppDefault ) Game_Unpause,
    Enabled( AppDefault ) Game_DefineExternals,
    Enabled( AppDefault ) Game_ApplyOptions
  );
}