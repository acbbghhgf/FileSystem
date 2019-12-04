#ifndef PICTURE_DATA_H
#define PICTURE_DATA_H

#include <iostream>
#include <string>
#include <vector>
#include <QDebug>


int make_encode_pic_save(std::string &encode_content_one_pic, int pic_num, int four_blocks_row_num, int four_blocks_col_num);
void make_picture_data(int four_blocks_row_num, int four_blocks_col_num);
QString get_pic_data(int four_blocks_row_num, int four_blocks_col_num, int pic_num);


#endif // PICTURE_DATA_H
