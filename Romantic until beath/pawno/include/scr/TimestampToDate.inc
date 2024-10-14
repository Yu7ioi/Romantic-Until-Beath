/*
-               Timestamp To Date converter	            -
-                    Made by Jochemd                    -
-          http://forum.sa-mp.com/member.php?u=580      -

native TimestampToDate(Timestamp, &year, &month, &day, &hour, &minute, &second, HourGMT, MinuteGMT = 0);
native DateToTimestamp(str[11]);
*/

/*
               时间戳转日期 
原作者Jochemd http://forum.sa-mp.com/member.php?u=580  BUG修复:YuCarl77([R_ST]Hygen)
很抱歉给各位带来的影响，此BUG经研究调查后发现在12月才会出现，经过细密的排查，最终修复了该严重漏洞。
轻微BUG:
	时间戳为0时，日显示为-1 即1970-1--1 00:00:00
	根本原因是原作者的写法不行，干脆重写部分代码
严重BUG(漏洞):
	用户登录时将显示最后一次登录时间，在时间戳转换时于if(day > MonthTimes[month][IsLeapYear(year)])处导致下标越界
科普:
	unix时间戳为格林威治时间1970年01月01日00时00分00秒起至现在的总秒数
	格林尼治标准时间（旧译格林威治平均时间或格林威治标准时间；英语：GreenwichMeanTime，GMT）
	指位于英国伦敦郊区的皇家格林尼治天文台的标准时间，因为本初子午线被定义在通过那里的经线。
BUG出现原因
	原作者的疏忽，之前一直计算错的月份，month此时已为12月。数组下标0~11对应1-12月，
	也就是之前一直算的是1月为2月，12月为13月 超出了下标导致计算直接错误但无报错
排查结果:
	时间戳在1606780800即北京时间2020年12月1日8时0分0秒及以后时间戳100%复现此问题（HourGMT）
	中国时区与英国时区相差8小时，以标准返回时间加了8小时候所以是12月1日 8时后出现 
修复时间:
2020年12月26日23时27分 
*/


#include <a_samp>
#include <sscanf2>

#define SPLITTER .

new MonthTimes[12][4] = 
{
	{ 31, 31, 2678400, 2678400 },
	{ 28, 29, 2419200, 2505600 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 },
	{ 30, 30, 2592000, 2592000 },
	{ 31, 31, 2678400, 2678400 }
};

// 优化下作者的代码
// stock IsLeapYear(year)
stock IsLeapYear(const year)
{
	// 原作者计算方法不太行啊
	// 能被400整除或能被4整除，并且不能被100整除的是闰年 而不是只被4整除
	// if(year % 4 == 0) return 1;
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

stock TimestampToDate(Timestamp, &year, &month, &day, &hour, &minute, &second, const HourGMT, const MinuteGMT = 0)
{
	for(year = 1970; ;year++)
	{
		new seconds = IsLeapYear(year) ? 31622400: 31536000;
		if (seconds > Timestamp) break;
		Timestamp -= seconds;
	}		
	for (month = 1; ; month++) {
		new seconds = MonthTimes[month-1][2 + IsLeapYear(year)];
		if (seconds > Timestamp) break;
		Timestamp -= seconds;
	}
	day = 1 + (Timestamp / 86400);
	Timestamp %= 86400;
	hour = HourGMT + (Timestamp / 3600);
	Timestamp %= 3600;
	minute = MinuteGMT + (Timestamp / 60);
	second = (Timestamp % 60);
	if(minute > 59)
	{
		minute = 0;
		hour ++;
	}
	if(hour > 23)
	{
		hour -= 24;
		day ++;
	}	
	if(day > MonthTimes[month-1][IsLeapYear(year)])
	{
		day = 1;
		month ++;
	}
	if(month > 12)
	{
		month = 1;
		year ++;
	}
	return 1;
}


stock DateToTimestamp(str[11])
{
	new date[3]; // date[0] = day		date[1] = month			date[2] = year
	if(!sscanf(str,"p<"#SPLITTER">ddd",date[0],date[1],date[2]))
	{
		new total = 0, tmp = 0;
		total += date[0] * 86400;
		if(date[1] == 2 && date[0] < 29) tmp = ((date[2] - 1968) / 4 - 2);
		else tmp = ((date[2] - 1968) / 4 - 1);
		total += tmp * 31622400;
		total += (date[2] - 1970 - tmp) * 31536000;
		for(new i = 1; i < date[1]; i ++) total += MonthTimes[i][0 + IsLeapYear(date[2])] * 86400;
		return total;
	}
	else return -1;
}


// 原作者的BUG写法
// stock TimestampToDateOld(Timestamp, &year, &month, &day, &hour, &minute, &second, HourGMT, MinuteGMT = 0)
// {
// 	new tmp = 2;
// 	year = 1970;
// 	month = 1;
// 	Timestamp -= 172800; // Delete two days from the current timestamp. This is necessary, because the timestamp retrieved using gettime() includes two too many days.
// 	for(;;)
// 	{
// 		if(Timestamp >= 31536000)
// 		{
// 			year ++;
// 			Timestamp -= 31536000;
// 			tmp ++;
// 			if(tmp == 4)
// 			{
// 				if(Timestamp >= 31622400)
// 				{
// 					tmp = 0;
// 					year ++;
// 					Timestamp -= 31622400;
// 				}
// 				else break;
// 			}
// 		}
// 		else break;
// 	}		
// 	for(new i = 0; i < 12; i ++)
// 	{
// 		if(Timestamp >= MonthTimes[i][2 + IsLeapYear(year)])
// 		{
// 			month ++;
// 			Timestamp -= MonthTimes[i][2 + IsLeapYear(year)];
// 		}
// 		else break;
// 	}
// 	day = 1 + (Timestamp / 86400);
// 	Timestamp %= 86400;
// 	hour = HourGMT + (Timestamp / 3600);
// 	Timestamp %= 3600;
// 	minute = MinuteGMT + (Timestamp / 60);
// 	second = (Timestamp % 60);
// 	if(minute > 59)
// 	{
// 		minute = 0;
// 		hour ++;
// 	}
// 	if(hour > 23)
// 	{
// 		hour -= 24;
// 		day ++;
// 	}	
// 	if(day > MonthTimes[month][IsLeapYear(year)])
// 	{
// 		day = 1;
// 		month ++;
// 	}
// 	if(month > 12)
// 	{
// 		month = 1;
// 		year ++;
// 	}
// 	return 1;
// }



// 原作者的BUG写法加上我新写的，给了注释的
// stock TimestampToDate(Timestamp, &year, &month, &day, &hour, &minute, &second, const HourGMT, const MinuteGMT = 0)
// {
// 	// new tmp = 2;
// 	// 1970暂定tmp为2 因为1972年tmp就是4了 每逢4年是闰年
// 	// new tmp = 2;
// 	// 起始年吧
// 	// year = 1970;
// 	// month = 1;
// 	// Timestamp -= 172800; // Delete two days from the current timestamp. This is necessary, because the timestamp retrieved using gettime() includes two too many days.
// 	// 翻译了下作者的话从当前时间戳中删除两天。这是必要的，因为使用gettime()检索的时间戳多了两天。
// 	// 但是如果我们是正常用户传参，不是通过gettime()的，没有多两天，不应该减去172800
// 	// 所以最简单的办法且顺着作者的意思的话就判断这个值是不是大于了两天吧
// 	// 也不是绝对化的解决这个问题 如果gettime还是可能出现问题
// 	// if(Timestamp > 172800) Timestamp -= 172800;

// 	// for(;;)
// 	// {
		
// 	// 	// 31536000是非闰年的一年的秒数
// 	// 	if(Timestamp >= 31536000)
// 	// 	{
			
// 	// 		year ++;
// 	// 		Timestamp -= 31536000;
// 	// 		tmp ++;
// 	// 		// 每逢4年是闰年
// 	// 		if(tmp == 4)
// 	// 		{
// 	// 			// 31622400是闰年的秒数
// 	// 			if(Timestamp >= 31622400)
// 	// 			{
					
// 	// 				// // 每逢4年是闰年 但依照原作者的意思。。。。。。如果是0的话 加到 4就花了5年了 变成每逢五年是闰年 神仙BUG
// 	// 				// tmp = 0;
// 	// 				tmp = 1;
// 	// 				// printf("闰年:%d",year);
// 	// 				year ++;
// 	// 				Timestamp -= 31622400;
// 	// 			}
// 	// 			else break;
// 	// 		}
// 	// 	}
// 	// 	else break;
// 	// }	

// 	for(year = 1970; ;year++)
// 	{
// 		// 31622400是闰年的秒数
// 		// 31536000是非闰年的一年的秒数
// 		new seconds = IsLeapYear(year) ? 31622400: 31536000;
// 		if (seconds > Timestamp) break;
// 		Timestamp -= seconds;
// 	}		
// 	// 这一部分应该是求月份，根据时间戳和对应是否为闰年检测
// 	// for(new i = 0; i < 12; i ++)
// 	// {
// 	// 	if(Timestamp >= MonthTimes[i][2 + IsLeapYear(year)])
// 	// 	{
// 	// 		month ++;
// 	// 		Timestamp -= MonthTimes[i][2 + IsLeapYear(year)];
// 	// 	}
// 	// 	else break;
// 	// }
	
// 	for (month = 1; ; month++) {
// 		new seconds = MonthTimes[month-1][2 + IsLeapYear(year)];
// 		if (seconds > Timestamp) break;
// 		Timestamp -= seconds;
// 	}
// 	// 减到一定量的时间戳后计算日 时 分 秒
// 	// 86400是一天是秒数


// 	day = 1 + (Timestamp / 86400);
// 	// 第几天就是第一天 + 这一天的时间戳秒数 / 86400 因为如果正好是第一天的话/86400就是0了
// 	Timestamp %= 86400;
	
// 	// 取模一天之后剩下的余数吧 
// 	// 小时就是0~23小时，即时间戳秒数 / 3600  前面加GMT是因为地区格林威治时间不一样 所以加上
// 	hour = HourGMT + (Timestamp / 3600);
// 	// 取模小时之后的余数
// 	// 3600是一个小时的秒数
// 	Timestamp %= 3600;
// 	// 分就是0~59分，即时间戳秒数 / 60  前面加GMT是因为地区格林威治时间不一样 所以加上
// 	minute = MinuteGMT + (Timestamp / 60);
// 	// 秒就是0~59秒，即时间戳秒数 取余 60分钟剩下的就是秒了
// 	second = (Timestamp % 60);
// 	// 如果分>59 就让分为0，小时 + 1 说白了也就60这种情况出现嘛
// 	if(minute > 59)
// 	{
// 		minute = 0;
// 		hour ++;
// 	}
// 	// 如果时>23 就让时为0，新的一天到了 说白了也就24小时这种情况出现嘛
// 	if(hour > 23)
// 	{
// 		hour -= 24;
// 		day ++;
// 	}	
// 	// if(day > MonthTimes[month][IsLeapYear(year)])
// 	// 原作者好像忘记减1了，如果是12月的话 数组就越界了 数组是1~12月 但是下标是0~11 没减1
// 	// 而且编译的时候是不会显示错误的，因为传入的值是变量，而不是Month直接就是12这个下标导致的下标越界
// 	// 测了半天
// 	// 纪念下 2020年12月26日晚
// 	if(day > MonthTimes[month-1][IsLeapYear(year)])
// 	{
// 		day = 1;
// 		month ++;
// 	}
// 	if(month > 12)
// 	{
// 		month = 1;
// 		year ++;
// 	}
// 	return 1;
// }
