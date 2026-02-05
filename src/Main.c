#include "/home/codeleaded/System/Static/Container/CVector.h"

#include "/home/codeleaded/System/Static/Library/ComponentPack.h"
#include "/home/codeleaded/System/Static/Library/ComponentML.h"
#include "/home/codeleaded/System/Static/Library/ComponentDefines.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/FileChooser.h"
#include "/home/codeleaded/System/Static/Library/MMenuSystem.h"

#define FILE_WIDTH              150
#define FILE_PADDINGX           10
#define FILE_OTHERRENDERS       1

#define FILE_SYNTAX_C           "/home/codeleaded/System/SyntaxFiles/C_Syntax.alxon"
#define FILE_SYNTAX_CPP         "/home/codeleaded/System/SyntaxFiles/Cpp_Syntax.alxon"
#define FILE_SYNTAX_LL          "/home/codeleaded/System/SyntaxFiles/LuaLike_Syntax.alxon"
#define FILE_SYNTAX_VBL         "/home/codeleaded/System/SyntaxFiles/VBLike_Syntax.alxon"
#define FILE_SYNTAX_AIX         "/home/codeleaded/System/SyntaxFiles/Aix_Syntax.alxon"
#define FILE_SYNTAX_ALX         "/home/codeleaded/System/SyntaxFiles/Alx_Syntax.alxon"
#define FILE_SYNTAX_OMML        "/home/codeleaded/System/SyntaxFiles/OMML_Syntax.alxon"
#define FILE_STARTPATH          "/home/codeleaded/Hecke/C/Gui_IDE_New/code"

MenuSystem menu;
MenuOption* preselected;
MenuOption* selected;

Scene scene;
ComponentPack cg;
ComponentML cml;
CStr fileinBuffer;
CVector filesOpen;

Editor* IDE_GetText(){
    Node* selected = scene.First;
    return (Editor*)selected->Memory;
}
void IDE_File_EventHandler(void* parent,Button* b,ButtonEvent* e){
    const int index = (int)((b->renderable.rect.p.x + 1.0f - 10.0f) / FILE_WIDTH);
    CStr path = *(CStr*)CVector_Get(&filesOpen,index);
    
    if(e->ButtonId == ALX_MOUSE_L && e->eid == EVENT_PRESSED){
        Editor* tb = IDE_GetText();
        if(tb){
            CStr content = Files_ReadT(path);
            if(content){
                Input_SetText(&tb->In,content);
                CStr_Set(&fileinBuffer,path);
                CStr_Free(&content);

                CStr type = Files_Type(path);
                
                if(CStr_Cmp(type,"c") || CStr_Cmp(type,"h"))            Editor_Syntax(tb,FILE_SYNTAX_C);
                else if(CStr_Cmp(type,"cpp") || CStr_Cmp(type,"hpp"))   Editor_Syntax(tb,FILE_SYNTAX_CPP);
                else if(CStr_Cmp(type,"ll"))                            Editor_Syntax(tb,FILE_SYNTAX_LL);
                else if(CStr_Cmp(type,"vbl"))                           Editor_Syntax(tb,FILE_SYNTAX_VBL);
                else if(CStr_Cmp(type,"aix"))                           Editor_Syntax(tb,FILE_SYNTAX_AIX);
                else if(CStr_Cmp(type,"alx"))                           Editor_Syntax(tb,FILE_SYNTAX_ALX);
                else if(CStr_Cmp(type,"omml"))                          Editor_Syntax(tb,FILE_SYNTAX_OMML);
                else                                                    Editor_Syntax(tb,NULL);
                
                CStr_Free(&type);
            }else{
                printf("[IDE]: File -> %s(%d) not found!\n",path,index);
            }
        }
    }
    if(e->ButtonId == ALX_MOUSE_R && e->eid == EVENT_PRESSED){
        Editor* tb = IDE_GetText();
        if(tb){
            if(fileinBuffer){
                Editor* tb = IDE_GetText();
                if(tb) Files_WriteT(fileinBuffer,tb->In.Buffer.Memory,tb->In.Buffer.size);
            }

            CVector_Remove(&filesOpen,index);
            Scene_RemoveQueue(&scene,b);
        }
    }
}
void IDE_EventHandler(void* parent,void* r,EventId* e){
    Scene* s = (Scene*)parent;
    Renderable* ra = (Renderable*)r;
    Component* c = Component_Scene_FindRR(&cg,ra);

    if(c){
        //Component_Print(&cg);
        //Scene_Print(&scene);
    }
}
void IDE_ExecuteSelect(){
    if(!selected) return;
    
    if(CStr_Cmp(selected->text,"open")){
        FileChooser fc = FileChooser_New("",FILE_STARTPATH,(NameTypePair[]){
            NameTypePair_New("All Files","*.*"),
            NameTypePair_Null()
        });
        if(fc){
            CStr_Set(&fileinBuffer,fc);
            CVector_Push(&filesOpen,(CStr[]){ CStr_Cpy(fileinBuffer) });
            
            CVector_Print(&filesOpen);
            
            printf("Choosen: '%s'\n",fileinBuffer);
            
            CStr name = Files_NameFull(fileinBuffer);
            const int count = scene.size - FILE_OTHERRENDERS;
            Scene_Add(&scene,(Button[]){ Button_New(
                NULL,
                name,
                (void(*)(void*,Label*,LabelEvent*))IDE_File_EventHandler,
                AlxFont_MAKE_HIGH(8,16),
                (Vec2){ 8,16 },
                (Rect){ 10.0f + count * FILE_WIDTH,0.0f,FILE_WIDTH - FILE_PADDINGX,20.0f },
                ALIGN_BORDER,
                0xFF444444,
                0xFFFFFFFF
                )},sizeof(Button)
            );
        
            CStr_Free(&name);
            FileChooser_Free(&fc);
        }
    }
    if(CStr_Cmp(selected->text,"save")){
        if(fileinBuffer){
            Editor* tb = IDE_GetText();
            if(tb) Files_WriteT(fileinBuffer,tb->In.Buffer.Memory,tb->In.Buffer.size);
        }
    } 
}

void Setup(AlxWindow* w){
    scene = Scene_New();

    menu = MenuSystem_New(
        AlxFont_MAKE_HIGH(16,32),
		"./assets/Pointer.png",
		"./assets/Fold.png",
		"./assets/FoldUp.png",
		"./assets/FoldDown.png",
		"./assets/Gray_Atlas.png",
		0xFF333333,32,0.0f,0.0f,0.0f
	);

	MenuSystem_Set(&menu,0,(int[]){   },	MenuOption_Make(0,"root","NONE",NULL,NULL,1,10));
	MenuSystem_Add(&menu,0,(int[]){   },	MenuOption_Make(1,"file","(file)",NULL,NULL,1,10));
	MenuSystem_Add(&menu,1,(int[]){ 0 },	MenuOption_New(2,"open","open a file",NULL,NULL));
	MenuSystem_Add(&menu,1,(int[]){ 0 },	MenuOption_New(3,"save","save current file",NULL,NULL));

	MenuSystem_Add(&menu,0,(int[]){   },	MenuOption_New(4,"edit","(edit)",NULL,NULL));
	MenuSystem_Add(&menu,0,(int[]){   },	MenuOption_New(5,"selection","(selection)",NULL,NULL));
	MenuSystem_Add(&menu,0,(int[]){   },	MenuOption_New(6,"view","(view)",NULL,NULL));
	MenuSystem_Add(&menu,0,(int[]){   },	MenuOption_New(7,"run","(run)",NULL,NULL));
	MenuSystem_Add(&menu,0,(int[]){   },	MenuOption_New(8,"terminal","(terminal)",NULL,NULL));
	MenuSystem_Add(&menu,0,(int[]){   },	MenuOption_New(9,"help","(help)",NULL,NULL));
	//MenuSystem_Add(&menu,1,(int[]){ 0 },	MenuOption_New(5,"x","0.0",&rect.p.x,(char *(*)(void*))Float_CStr));
	MenuSystem_Step(&menu);
	preselected = NULL;
	selected = NULL;

    fileinBuffer = NULL;
    filesOpen = CVector_New(sizeof(CStr),(void*)CStr_Free,NULL,(void*)CStr_PCmp,(void*)CStr_print);
}
void Update(AlxWindow* w){
    if(Stroke(ALX_KEY_ESC).PRESSED){
        if(menu.trace.size == 0)    MenuSystem_Step(&menu); 
        else                        MenuSystem_Clear(&menu);
    }

    if(menu.trace.size == 0){
        Scene_Update(&scene,window.Strokes,GetMouse(),GetMouseBefore());

        Node* n = scene.First->Next;
        for(int i = FILE_OTHERRENDERS;i<scene.size;i++){
            Renderable* rend = (Renderable*)n->Memory;
            rend->rect.p.x = 10.0f + (i - FILE_OTHERRENDERS) * FILE_WIDTH;
            n = n->Next;
        }
    }else{
        if(Stroke(ALX_MOUSE_L).PRESSED){
			MMenuSystem_SelectPoint(&menu,GetMouse());
			preselected = selected;
			selected = MenuSystem_Select(&menu);
            if(preselected == selected) IDE_ExecuteSelect();
		}
        /*
        if(Stroke(ALX_KEY_ENTER).PRESSED){
	    	selected = MenuSystem_Select(&menu);
            IDE_ExecuteSelect();
        }
        if(Stroke(ALX_KEY_UP).PRESSED){
	    	MenuSystem_Up(&menu);
	    }
	    if(Stroke(ALX_KEY_DOWN).PRESSED){
	    	MenuSystem_Down(&menu);
	    }
	    if(Stroke(ALX_KEY_LEFT).PRESSED){
	    	MenuSystem_Left(&menu);
	    }
	    if(Stroke(ALX_KEY_RIGHT).PRESSED){
	    	MenuSystem_Right(&menu);
	    }
        */
    }

	MenuSystem_Update(&menu);

	Clear(BLACK);

	Scene_Render(WINDOW_STD_ARGS,&scene);
    //MenuSystem_Render(WINDOW_STD_ARGS,&menu);
	MMenuSystem_Render(WINDOW_STD_ARGS,&menu);
}
void Resize(AlxWindow* w){
	Editor* e = IDE_GetText();
    e->renderable.rect.d.x = (float)(GetWidth() - e->renderable.rect.p.x) * 0.99f;
    e->renderable.rect.d.y = (float)(GetHeight() - e->renderable.rect.p.y) * 0.99f;
}
void Delete(AlxWindow* w){
	Scene_Free(&scene);
    Component_Free(&cg);
    ComponentML_Free(&cml);
    MenuSystem_Free(&menu);

    CStr_Free(&fileinBuffer);
    CVector_Free(&filesOpen);
}

int main(int argc,char** argv){
    if(CreateX("IDE - New",1900,1000,1,1,Setup,Update,Delete,Resize))
        Start();
    return 0;
}