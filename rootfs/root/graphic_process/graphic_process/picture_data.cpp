#include "picture_data.h"


static QString save_pic_data[20];


unsigned char getXsChar(int a, int b, int c)
{
    unsigned char ch = 0;
    if (a > 15)
    {
        ch = c * 16 + c;
    }
    else
    {
        c = (a + b) % 16;
        ch = c * 16 + c;
    }
    return ch;
}


void make_picture_data(int four_blocks_row_num, int four_blocks_col_num)
{
    std::vector<std::string> model_strs;
    for (int i = 0; i < 20; ++i)
    {
        std::string model_str_one;
        if (i > 15)
        {
            for (int j = 0; j < four_blocks_row_num * four_blocks_col_num / 2; ++j)
            {
                int temp = 0;
                switch (i)
                {
                case 16:
                    temp = 0;
                    break;
                case 17:
                    temp = 6;
                    break;
                case 18:
                    temp = 9;
                    break;
                default:
                    break;
                }
                unsigned char ch;
                ch = getXsChar(i, 0, temp);
                model_str_one.push_back(ch);
            }
        }
        else
        {
            for (int j = 0; j < four_blocks_row_num; j++)
            {
                unsigned char ch = getXsChar(i, j, 0);
                for (int k = 0; k < four_blocks_col_num / 2; k++)
                {
                    model_str_one.push_back(ch);
                }
            }
        }
        model_strs.push_back(model_str_one);
    }
    for (auto it = model_strs.cbegin(); it != model_strs.cend(); ++it)
    {
        QString tmp = QString::fromStdString(const_cast<std::string &>(*it));
//        qDebug() << "picture : " << tmp;
        make_encode_pic_save(const_cast<std::string &>(*it), static_cast<int>(it-model_strs.cbegin()), four_blocks_row_num, four_blocks_col_num);
    }
    qDebug() << "picture 18 data:" << save_pic_data[18];

}


int make_encode_pic_save(std::string &encode_content_one_pic, int pic_num, int four_blocks_row_num, int four_blocks_col_num)
{
    if (encode_content_one_pic.size() > four_blocks_row_num * four_blocks_col_num)
    {
        qDebug("error: encoded content for one pic is more than the limit %d",
               four_blocks_row_num * four_blocks_col_num);
        return -1;
    }
    for (auto it = encode_content_one_pic.cbegin(); it != encode_content_one_pic.cend();)
    {
        for (int cur_y = 0; cur_y < four_blocks_row_num; ++cur_y)
        {
            for (int cur_x = 0; cur_x < four_blocks_col_num; cur_x += 2)
            {
                if (it != encode_content_one_pic.cend())
                {/*
                    charShow(start_x, start_y, cur_x, cur_y, draw_width, draw_height, four_blocks_col_num,
                             four_blocks_row_num, static_cast<unsigned char>(*it), pix_one);*/
                    save_pic_data[pic_num].push_back(static_cast<unsigned char>(*it));
                    ++it;
                }
                else
                {
                    break;
                }
            }
        }
    }
    return 0;
}

QString get_pic_data(int pic_num)
{
    QString tmp = save_pic_data[pic_num];
    return tmp;
}

