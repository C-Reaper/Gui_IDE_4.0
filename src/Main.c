#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TextEditor.h"


TextEditor editor;

void Resize(AlxWindow* w){
    editor.rect.p.x = (float)GetWidth() * 0.1f;
    editor.rect.p.y = (float)GetHeight() * 0.01f;
    editor.rect.d.x = (float)(GetWidth() - editor.rect.p.x) * 0.99f;
    editor.rect.d.y = (float)(GetHeight() - editor.rect.p.y) * 0.99f;
}
void Setup(AlxWindow* w){
    editor = TextEditor_New(Rect_New(
        (Vec2){ 0.0f,0.0f },
        (Vec2){ 0.0f,0.0f }
    ));
    TextEditor_Load(&editor,"./src/Main.c");

    Resize(w);
}
void Update(AlxWindow* w){
    TextEditor_Update(&editor,w->Strokes,GetMouse());
    TextEditor_DefaultReact(&editor);

	Clear(DARK_GRAY);

	TextEditor_Render(&editor,WINDOW_STD_ARGS);
}
void Delete(AlxWindow* w){
	TextEditor_Free(&editor);
}

int main(int argc,char** argv){
    if(CreateX("IDE - 4.0",1900,1000,1,1,Setup,Update,Delete,Resize))
        Start();
    return 0;
}