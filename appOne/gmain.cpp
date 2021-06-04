#if 1//�������Ń^�C�v���������͂������P�ɂ��Ă�������
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
    time_t stdTime;//�W�����������󂯎��Ȃ��A�W�������Ƃ͂P�X�V�O�N�P������O�����牽�b�o������
    struct tm local;//���[�J�����Ԃɕϊ����邽�߂�
    //�����Ƃ����\���̂������Ă����Ƀ��[�J�����Ԃɕϊ��������ʂ������
    while (notQuit) {
        time(&stdTime);//�C�M���X���ԁi�W�����Ԃ��󂯎��
        localtime_s(&local, &stdTime);//�����Ń��[�J���\���̂̒���
        //�ϊ����ē����Ă���
        second = local.tm_sec;//�ϊ������l�����ꂼ��ɏ�������
        minute = local.tm_min;
        hour = local.tm_hour;

        //second += 1;
        //if (second >= 60) { minit += 1;second = 0; }
        //if (minit >= 60) { hour += 1;minit = 0; }

        //���v�̕�
        clear(200);
        strokeWeight(50);
        stroke(252, 200, 100);
        fill(255,255,255,180);
        circle(setX, setY, r * 2);

        //�b�j
        deg = 6 * second;
        y = -cos(deg)*370;//���v�̕b�j���ォ��n�߂����̂�cos�ɂ�������
        x = sin(deg)*370;//���̂܂܂��Ɖ��ɗ���̂Ł[������ɂ���
        strokeWeight(5);
        stroke(3,252,227);
        line(width / 2, height / 2, x + width / 2, y + height / 2);

        //���j
        deg = 6 * minute + 6 * second / 60;
        x = sin(deg) * 370;
        y = -cos(deg) * 370;
        strokeWeight(20);
        line(width / 2, height / 2, x + width / 2, y + height / 2);

        //���j�@
        deg = 30 * hour + 30 * minute / 60;
        x = sin(deg) * 250;
        y = -cos(deg) * 250;
        strokeWeight(50);
        line(width / 2, height / 2, x + width / 2, y + height / 2);
        strokeWeight(20);

        //���ԕ\��
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
//���݈ȉ��̃R�[�h���L���ɂȂ��Ă��܂�
#include"libOne.h"
#include<time.h>
//�b�j�A���j�A���j�̃f�[�^�i�p��Őj��hand�j
struct HAND {
    float deg, scale, satulation, value, weight;
};
//���v�f�[�^
struct CLOCK {
    float num, diameter, radius;
    float sx=0, sy=0, hue=0;
    int hour=0, minute=0, second=0;
    struct HAND hourHand   = { 0, 0.56f, 200,  255, 40 };
    struct HAND minuteHand = { 0, 0.8f,  100,  255, 30 };
    struct HAND secondHand = { 0, 0.8f,  255,  128, 10 };
};
//���ݎ������f�[�^�ɃZ�b�g����
void setTime(struct CLOCK& c) {
    if (!isPress(KEY_SPACE)) {
        //���A���A�b���擾
        time_t stdTime= time(NULL);//�W�����Ԃ��擾
        struct tm local;
        localtime_s(&local, &stdTime);//���[�J��������
        c.hour = local.tm_hour;
        c.minute = local.tm_min;
        c.second = local.tm_sec;
    }
    else {
        //�X�y�[�X�������Ɛj����������
        c.second += 5;
        if (c.second >= 60) { c.minute++; c.second = 0; }
        if (c.minute >= 60) { c.hour += 1; c.minute = 0; }
        if (c.hour >= 24) {c.hour = 0;}
    }
}
//�e�j�̕`��Ŏg�p����֐�
void draw(const struct CLOCK& c, const struct HAND& h) {
    strokeWeight(h.weight / c.num);
    stroke(c.hue, h.satulation, h.value);
    float ex = c.sx + sin(h.deg) * (c.radius-30) * h.scale;
    float ey = c.sy - cos(h.deg) * (c.radius-30) * h.scale;
    line(c.sx, c.sy, ex, ey);
}
//���v�̕`��
void draw(struct CLOCK& c) {
    //�~��
    noStroke();
    fill(c.hue, 200, 255);
    circle(c.sx, c.sy, c.diameter);
    fill(c.hue, 30, 255);
    circle(c.sx, c.sy, c.diameter-150/c.num);
    //��
    c.hourHand.deg = (c.hour * 30) + 30 * (c.minute / 60.0f);
    draw(c, c.hourHand);
    //��
    c.minuteHand.deg = (c.minute * 6) + 6 * (c.second / 60.0f);
    draw(c, c.minuteHand);
    //�b
    c.secondHand.deg = c.second * 6;
    draw(c, c.secondHand);
    //�b�j�̗v
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