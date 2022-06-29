#include "DxLib.h"

#define WIN_W 800 
#define WIN_H 600
#define MAX_READ 2000
#define LANE_NUM 4
#define NOTE_NUM 1000
#define NOTE_WIDTH 72
#define NOTE_HEIGHT 20
#define JUDGE_Y 500


//キーの値を保持
static const int KEYS[LANE_NUM] = { KEY_INPUT_X,KEY_INPUT_C,KEY_INPUT_N,KEY_INPUT_M };

//譜面データ関連
struct MUGIC_DATA
{
    double bpm = 120;
    double offset = 0;
    char song_name[MAX_READ] = "unknown";
    char music_file_path[MAX_READ];
    double perfect_times[LANE_NUM][NOTE_NUM] = { 0 };
    int perfect_time_size[LANE_NUM] = { 0 };
};

//譜面データの確認
bool loadHumenOptions(MUGIC_DATA* music_data, FILE* fp)
{
    char str[MAX_READ], * next_token = NULL, tstr[MAX_READ];
    while ((fgets(str, MAX_READ, fp)) != NULL)
    {
        strncpy_s(tstr, MAX_READ, str, 6);
        if (strcmp(tstr, "#START") == 0)
        {
            return true;
        }

        char* first = strtok_s(str, ":", &next_token);
        if (first == NULL) continue;

        if (strcmp(first, "BPM") == 0)
        {
            char* second = strtok_s(NULL, ":", &next_token);
            music_data->bpm = atof(second);
        }
        else if (strcmp(first, "OFFSET") == 0)
        {
            char* second = strtok_s(NULL, ":", &next_token);
            music_data->offset = atof(second);
        }
        else if (strcmp(first, "TITLE") == 0)
        {
            char* second = strtok_s(NULL, "\n", &next_token);
            strcpy_s(music_data->song_name, second);
        }
        else if (strcmp(first, "WAVE") == 0)
        {
            char* second = strtok_s(NULL, "\n", &next_token);
            strcpy_s(music_data->music_file_path, second);
        }
    }
    return false;
}

//譜面データの照合
void loadHumen(MUGIC_DATA* music_data, FILE* fp)
{
    char line[MAX_READ], * next_token = NULL, delim[4] = ", \n";

    for (int col = 0; col < LANE_NUM; col++)
    {
        music_data->perfect_time_size[col] = 0;
    }

    for (int row = 0; (fgets(line, MAX_READ, fp)) != NULL; row++)
    {
        char* str = strtok_s(line, delim, &next_token);
        if (str == NULL) continue;

        for (int col = 0; str != NULL; col++)
        {
            int length = strlen(str);
            for (int i = 0; i < length; i++)
            {
                if ('1' <= str[i] && str[i] <= '9')
                {
                    music_data->perfect_times[col][music_data->perfect_time_size[col]++] = 60.0 * 4 * (row + (double)i / length) / music_data->bpm - music_data->offset;
                }
            }
            str = strtok_s(NULL, delim, &next_token);
        }
    }
}

//譜面データの代入
bool loadHumenData(MUGIC_DATA* music_data, const char* file_name)
{
    FILE* fp;

    if ((fopen_s(&fp, file_name, "r")) != 0 || fp == 0)
    {
        return false;
    }

    bool loadable = loadHumenOptions(music_data, fp);

    if (loadable) loadHumen(music_data, fp);

    fclose(fp);
    return true;
}


//ノーツ構造体
struct NOTE
{
    bool flag = false;
    float x = 0.f;
    float y = 0.f;
};

void initNotes(int perfect_time_size[LANE_NUM], NOTE notes[LANE_NUM][NOTE_NUM])
{
    for (int col = 0; col < LANE_NUM; col++)
    {
        int lane_length = perfect_time_size[col];
        for (int row = 0; row < lane_length; row++)
        {
            notes[col][row].flag = true;
            notes[col][row].x = 200.f + 150.f * col;
        }
    }
}

//ノーツ座標更新
void updateNotes(double current_time, double perfect_times[LANE_NUM][NOTE_NUM], NOTE notes[LANE_NUM][NOTE_NUM])
{
    for (int col = 0; col < LANE_NUM; col++)
    {
        for (int row = 0; row < NOTE_NUM; row++)
        {
            if (notes[col][row].flag)
                notes[col][row].y = WIN_H * (float)(current_time - perfect_times[col][row]) / 2 + JUDGE_Y;
        }
    }

    //判定範囲外に出たノーツの削除
    for (int col = 0; col < LANE_NUM; col++)
    {
        for (int row = 0; row < NOTE_NUM; row++)
        {
            if (notes[col][row].flag && WIN_H + NOTE_HEIGHT < notes[col][row].y && 0.3 < current_time - perfect_times[col][row])
            {
                notes[col][row].flag = false;
            }
        }
    }
}

//判定範囲内でキーが押されたときのノーツ削除
void judgeNotes(double current_time, double perfect_times[LANE_NUM][NOTE_NUM], NOTE notes[LANE_NUM][NOTE_NUM], char buf[256])
{
    for (int col = 0; col < LANE_NUM; col++)
    {
        for (int row = 0; row < NOTE_NUM; row++)
        {
            if (buf[KEYS[col]] == 1 && notes[col][row].flag && -0.3 < current_time - perfect_times[col][row] && current_time - perfect_times[col][row] < 0.3)
            {
                notes[col][row].flag = false;
            }
        }
    }
}

void drawNotes(NOTE notes[LANE_NUM][NOTE_NUM])
{
    for (int col = 0; col < LANE_NUM; col++)
    {
        for (int row = 0; row < NOTE_NUM; row++)
        {
            if (notes[col][row].flag)
            {
                DrawBoxAA(notes[col][row].x, notes[col][row].y - NOTE_HEIGHT / 2, notes[col][row].x + NOTE_WIDTH, notes[col][row].y + NOTE_HEIGHT / 2, GetColor(0, 206, 209), TRUE);
            }
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MUGIC_DATA music_data;
    NOTE notes[LANE_NUM][NOTE_NUM];
    char buf[256]; //キーの格納配列

    //ウィンドウの表示関連
    SetMainWindowText("音ゲー（仮）");
    ChangeWindowMode(TRUE);
    SetGraphMode(WIN_W, WIN_H, 32);
    SetWindowSizeChangeEnableFlag(FALSE);
    if (DxLib_Init() == -1)
    {
        return -1;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    loadHumenData(&music_data, "otoge_main\\note.txt");

    initNotes(music_data.perfect_time_size, notes);


    //音源関連
    int bgmHandle = LoadSoundMem("otoge_main\\bgm_maoudamashii_fantasy13.wav");
    PlaySoundMem(bgmHandle, DX_PLAYTYPE_BACK);

    LONGLONG start_count = GetNowHiPerformanceCount();

    //画面表示関連
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0)
    {
        GetHitKeyStateAll(buf);
        if (buf[KEY_INPUT_ESCAPE] == 1)
        {
            break;
        }

        LONGLONG now_count = GetNowHiPerformanceCount();
        double current_time = (now_count - start_count) / 1000000.0;

        updateNotes(current_time, music_data.perfect_times, notes);

        judgeNotes(current_time, music_data.perfect_times, notes, buf);

        //判定ラインの描画
        DrawLine(0, JUDGE_Y, WIN_W, JUDGE_Y, GetColor(255, 0, 255));

        //時間、bpm、offset、曲名、音源ファイル名の描画
        DrawFormatString(10, 10, GetColor(255, 0, 255 ), "t:%f\nbpm:%f\noffset:%f\n%s\n%s", current_time, music_data.bpm, music_data.offset, music_data.song_name, music_data.music_file_path);

        drawNotes(notes);
    }

    DxLib_End();
    return 0;
}