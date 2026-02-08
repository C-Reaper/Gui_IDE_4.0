#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Explorer.h"
#include "/home/codeleaded/System/Static/Library/TextEditor.h"

Explorer ex;
TextEditor editor;

void Resize(AlxWindow* w){
    ex.r.p.x = (float)GetWidth() * 0.01f;
    ex.r.p.y = (float)GetHeight() * 0.01f;
    ex.r.d.x = (float)GetWidth() * (0.2f - 0.01f * 2);
    ex.r.d.y = (float)GetHeight() * (1.0f - 0.01f * 2);

    editor.rect.p.x = (float)GetWidth() * 0.21f;
    editor.rect.p.y = (float)GetHeight() * 0.01f;
    editor.rect.d.x = (float)GetWidth() * (0.8f - 0.01f * 2);
    editor.rect.d.y = (float)GetHeight() * (1.0f - 0.01f * 2);
}
void Setup(AlxWindow* w){
    ex = Explorer_New("..",Rect_New(
		(Vec2){ 0.0f,0.0f },
		(Vec2){ 0.0f,0.0f }
	));

    editor = TextEditor_New(Rect_New(
        (Vec2){ 0.0f,0.0f },
        (Vec2){ 0.0f,0.0f }
    ));
    TextEditor_Load(&editor,"./src/Main.c");

    Resize(w);
}
void Update(AlxWindow* w){
	if(Stroke(ALX_MOUSE_L).PRESSED){
		Explorer_File* ef = Explorer_Select(&ex,w->Strokes,GetMouse());
		if(ef && Files_isFile(ef->path)){
			printf("[Explorer_File]: Open -> '%s'\n",ef->path);
            TextEditor_Load(&editor,ef->path);
		}
	}
    
    Explorer_Update(&ex,w->Strokes,GetMouse());
    
    TextEditor_Update(&editor,w->Strokes,GetMouse());
    TextEditor_DefaultReact(&editor);

	Clear(DARK_GRAY);

	Explorer_Render(&ex,WINDOW_STD_ARGS);
	TextEditor_Render(&editor,WINDOW_STD_ARGS);
}
void Delete(AlxWindow* w){
	Explorer_Free(&ex);
	TextEditor_Free(&editor);
}

int main(int argc,char** argv){
    if(CreateX("IDE - 4.0",1900,1000,1,1,Setup,Update,Delete,Resize))
        Start();
    return 0;
}