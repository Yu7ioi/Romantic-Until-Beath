#if !defined n_sampmailjs_include
    #define n_sampmailjs_include
#endif

#include a_http
#include strlib

#define SAMPMAILJS_RESPONSE_OK 200
#define SAMPMAILJS_RESPONSE_FORBIDEN 403
#define SAMPMAILJS_RESPONSE_ERROR 400
#define SAMPMAILJS_RESPONSE_NOTFOUND 404

// #define SAMPMAILJS_URL "127.0.0.1:8080" // IP + PORT of the server running the NodeJS script
// #define SAMPMAILJS_PASSWORD "changeme" // This password should be the same as the NodeJS script
// #define SAMPMAILJS_CONSOLELOG 0 // Log some information on console
// #define SAMPMAILJS_MAXPOSTDATA 1024 // Cellsize of the variable that holds the data sent to the script. Lower for small emails, higher for more extense emails

new uniqueID = 0;

function HandleHTTPResponse(index, response_code, data[]) {
    uniqueID++;

    #if SAMPMAILJS_CONSOLELOG == 1
		printf("\n[SAMPMAILJS] Server returned a response %d", response_code);
    #endif

    if (response_code == SAMPMAILJS_RESPONSE_OK) {
  		#if SAMPMAILJS_CONSOLELOG == 1
  			printf("\n[SAMPMAILJS] Email %d sent successfully", uniqueID);
  		#endif
		SendClientMessage(index, Color_Red, "[安全]验证码已经成功发送到你的邮箱,请查看邮箱进行验证");
		return response_code;
    }
	new text[128];
	format(text, sizeof(text), "[错误]安全中心邮箱验证系统发送失败,错误代码[%d]", response_code); 
	if(PlayerInfo[index][Confirmed] == 0) format(PlayerInfo[index][Email], MAX_EMAIL_LENGTH, "null"); //重置邮箱
	PlayerInfo[index][yzwrong] = 0;
	SendClientMessage(index, Color_Red, text);
	return response_code;
}

stock SendEmail(const playerid, const name[], const to[], const subject[], const text[], bool:isTemplate = false, const templateName[] = "default.html") {
	new string[128], postData[SAMPMAILJS_MAXPOSTDATA];

	if(isTemplate) {
		format(string, sizeof string, "%s/sampmail.js?pw=%s&action=sendmtmp", SAMPMAILJS_URL, SAMPMAILJS_PASSWORD);
		format(postData, sizeof postData, "%s|%s|%s|%s|%s", name, to, subject, text, templateName);
	}
	else {
 		format(string, sizeof string, "%s/sampmail.js?pw=%s&action=sendm", SAMPMAILJS_URL, SAMPMAILJS_PASSWORD);
	  	format(postData, sizeof postData, "%s|%s|%s|%s", name, to, subject, text);
	}
	// 不用这个utf8转换了 中文gbk比较特殊了
  	// utf8encode(postData, postData);
	// return HTTP(uniqueID, HTTP_POST, string, postData, "HandleHTTPResponse");
	return HTTP(playerid, HTTP_POST, string, postData, "HandleHTTPResponse");
}
