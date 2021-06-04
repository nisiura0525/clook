#if 1//←自分でタイプしたい時はここを１にしてください
#include"libOne.h"
#include<time.h>
void gmain() {
    window(1000, 1000); 
    float x, y, deg = 0,r=0;
    float second = 0, minute = 0, hour = 0;
    float setX = 0, setY = 0,p=0;
    setX = width/2;
    setY = height/2;
    r = 400;
    angleMode(DEGREES);
    time_t stdTime;//標準時刻しか受け取れない、標準時刻とは１９７０年１月一日０時から何秒経ったか
    struct tm local;//ローカル時間に変換するために
    //ｔｍという構造体があってそこにローカル時間に変換した結果をいれる
    while (notQuit) {
        time(&stdTime);//イギリス時間（標準時間を受け取る
        localtime_s(&local, &stdTime);//ここでローカル構造体の中に
        //変換して入ってくる
        second = local.tm_sec;//変換した値をそれぞれに書き込む
        minute = local.tm_min;
        hour = local.tm_hour;

        //second += 1;
        //if (second >= 60) { minit += 1;second = 0; }
        //if (minit >= 60) { hour += 1;minit = 0; }

        //時計の淵
        clear(200);
        strokeWeight(50);
        stroke(252, 200, 100);
        fill(255,255,255,180);
        circle(setX, setY, r * 2);

        //秒針
        deg = 6 * second;
        y = -cos(deg)*370;//時計の秒針を上から始めたいのでcosにｙをいれ
        x = sin(deg)*370;//そのままだと下に来るのでーをつけ上にする
        strokeWeight(5);
        stroke(3,252,227);
        line(width / 2, height / 2, x + width / 2, y + height / 2);

        //分針
        deg = 6 * minute + 6 * second / 60;
        x = sin(deg) * 370;
        y = -cos(deg) * 370;
        strokeWeight(20);
        line(width / 2, height / 2, x + width / 2, y + height / 2);

        //時針　
        deg = 30 * hour + 30 * minute / 60;
        x = sin(deg) * 250;
        y = -cos(deg) * 250;
        strokeWeight(50);
        line(width / 2, height / 2, x + width / 2, y + height / 2);
        strokeWeight(20);

        //時間表示
        for (int i = 1;i <= 12;i++) {
             p += 30;
            y = -cos(p) * 400;
            x = sin(p) * 400;
            fill(0);
            textSize(50);
            text(i, setX + x -16, setY + y+30 );
        }
       

    }
}

#else
//現在以下のコードが有効になっています
#include"libOne.h"
#include<time.h>
//秒針、分針、時針のデータ（英語で針はhand）
struct HAND {
    float deg, scale, satulation, value, weight;
};
//時計データ
struct CLOCK {
    float num, diameter, radius;
    float sx=0, sy=0, hue=0;
    int hour=0, minute=0, second=0;
    struct HAND hourHand   = { 0, 0.56f, 200,  255, 40 };
    struct HAND minuteHand = { 0, 0.8f,  100,  255, 30 };
    struct HAND secondHand = { 0, 0.8f,  255,  128, 10 };
};
//現在時刻をデータにセットする
void setTime(struct CLOCK& c) {
    if (!isPress(KEY_SPACE)) {
        //時、分、秒を取得
        time_t stdTime= time(NULL);//標準時間を取得
        struct tm local;
        localtime_s(&local, &stdTime);//ローカル時刻に
        c.hour = local.tm_hour;
        c.minute = local.tm_min;
        c.second = local.tm_sec;
    }
    else {
        //スペースを押すと針が速く動く
        c.second += 5;
        if (c.second >= 60) { c.minute++; c.second = 0; }
        if (c.minute >= 60) { c.hour += 1; c.minute = 0; }
        if (c.hour >= 24) {c.hour = 0;}
    }
}
//各針の描画で使用する関数
void draw(const struct CLOCK& c, const struct HAND& h) {
    strokeWeight(h.weight / c.num);
    stroke(c.hue, h.satulation, h.value);
    float ex = c.sx + sin(h.deg) * (c.radius-30) * h.scale;
    float ey = c.sy - cos(h.deg) * (c.radius-30) * h.scale;
    line(c.sx, c.sy, ex, ey);
}
//時計の描画
void draw(struct CLOCK& c) {
    //円盤
    noStroke();
    fill(c.hue, 200, 255);
    circle(c.sx, c.sy, c.diameter);
    fill(c.hue, 30, 255);
    circle(c.sx, c.sy, c.diameter-150/c.num);
    //時
    c.hourHand.deg = (c.hour * 30) + 30 * (c.minute / 60.0f);
    draw(c, c.hourHand);
    //分
    c.minuteHand.deg = (c.minute * 6) + 6 * (c.second / 60.0f);
    draw(c, c.minuteHand);
    //秒
    c.secondHand.deg = c.second * 6;
    draw(c, c.secondHand);
    //秒針の要
    strokeWeight(20 / c.num);
    point(c.sx, c.sy);
}

void gmain() {
    window(1080, 1080, full);
    angleMode(DEGREES);
    colorMode(HSV);
    struct CLOCK clock;
    clock.num = 1;
    clock.diameter = height / clock.num;
    clock.radius = clock.diameter / 2;
    while (notQuit) {
        if (isPress(KEY_D) || isPress(KEY_A) ||
            isTrigger(KEY_W) || isTrigger(KEY_S)) {
            if (isPress(KEY_D) )++clock.num;
            if (isPress(KEY_A) && clock.num > 1)clock.num--;
            if (isTrigger(KEY_W))clock.num++;
            if (isTrigger(KEY_S) && clock.num > 1)clock.num--;
            clock.diameter = height / clock.num;
            clock.radius = clock.diameter / 2;
        }
        setTime(clock);
        clear(55);
        for (int j = 0; j < clock.num; j++) {
            clock.sy = clock.radius + clock.diameter * j;
            for (int i = 0; i < clock.num; i++) {
                clock.sx = clock.radius + clock.diameter * i;
                clock.hue = 360.0f / (clock.num*clock.num) * (j * clock.num + i)+25;
                draw(clock);
            }
        }
    }
}
#endif