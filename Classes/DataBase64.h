
/********************************************************************
 *    文件名  :    DATABASE64.H
 *    作者    :    
 *    创建时间:    2013/1/2 15:58
 *    文件描述:    结合database64算法来加密存档数据
 *********************************************************************/

#ifndef _H_DATABASE64_H_
#define _H_DATABASE64_H_

#include <string>

std::string saveData(unsigned char const* , unsigned int len);
std::string parseData(std::string const& s);

#endif // _H_DATABASE64_H_
