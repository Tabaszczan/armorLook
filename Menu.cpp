// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
    Array<oCItem*> Menu::GetArmors() {
        Array<oCItem*> armors;
        auto c_item = parser->GetIndex(oCItem::classDef->scriptClassName);
        for (int i = 0; i < parser->symtab.GetNumInList(); i++) {
            zCPar_Symbol* symbol = parser->symtab.table[i];

            if (i == parser->instance_help) {
                continue;
            }

            if (symbol->type != zPAR_TYPE_INSTANCE) {
                continue;
            }

            //  Instance isn't global scope
            if (symbol->name.HasWord(".")) {
                continue;
            }
            auto baseClass = parser->GetBaseClass(symbol);
            if (baseClass != c_item) {
                continue;
            }
            oCItem* item = static_cast<oCItem*>(ogame->GetGameWorld()->CreateVob(zVOB_TYPE_ITEM, i));
            if (item->mainflag == ITM_CAT_ARMOR) {
                armors.Insert(item);
            }
            else {
                item->Release();
            }
        }
        return armors;
    };

}