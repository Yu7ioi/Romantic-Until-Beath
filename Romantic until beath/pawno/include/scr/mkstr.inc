//支持处理中文字符串的strlen,strcmp,strfind
//作者:mk124
//连接:http://www.gtaun.net/thread-682-1-7.html
// #define strlen mk_strlen
// #define strcmp mk_strcmp
// #define strfind mk_strfind
stock mk_strlen(const string[])
{
        new i;
        while( string[i]!=0 )i++;
        return i;
}
stock mk_strcmp(const string1[], const string2[], bool:ignorecase=false, length=cellmax)
{
        new mbcs;

        for( new i=0; i<length; i++ )
        {
                new c1=string1[i], c2=string2[i];
                if( c1 < 0 ) c1+=256;
                if( c2 < 0 ) c2+=256;
                if( ignorecase && c1 <= 0x7F && c2 <= 0x7F && mbcs==0 )
                {
                        c1 = tolower(c1);
                        c2 = tolower(c2);
                }
                if(mbcs==1) mbcs=0;
                else if( c1 > 0x7F || c2 > 0x7F ) mbcs=1;

                if( c1 != c2 || (c1==0 && c2==0) ) return c1-c2;
        }

        return 0;
}
stock mk_strfind(const string[], const sub[], bool:ignorecase=false, pos=0)
{
        new len = strlen(string), sublen = strlen(sub), count, mbcs, i;

        for( i=pos; i<=len && sublen-count<=len-i; i++ )
        {
                new c1=string[i], c2=sub[count];
                if( c1 < 0 ) c1+=256;
                if( c2 < 0 ) c2+=256;
                if( ignorecase && mbcs==0 )
                {
                        c1 = tolower(c1);
                        c2 = tolower(c2);
                }
                if(mbcs==1) mbcs=0;
                else if( c1 > 0x7F || c2 > 0x7F ) mbcs=1;

                if( c1 == c2 )
                {
                        if( count == sublen-1 ) return i-count;
                        count++;
                }
                else count = 0;
        }
        return -1;
}

//---strtok大数组版本
stock strtok(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}

	new offset = index;
	new result[128];
	while ((index < length) && (string[index] > ' ') && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}
//--strrest(碰到13(回车)键就返回的strrest
stock strrest(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}
	new offset = index;
	new result[128];
	while ((index < length) && ((index - offset) < (sizeof(result) - 1)))
	{
		if(string[index]==13) break;
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}
//--检测字符串
//--作者:jeff_hardy
//--连接:http://www.gtaun.net/thread-15397-1-5.html
//--返回类型
/*
返回值:
0:空字符串
1:纯数字字符串
2:纯字母字符串
3:纯符号字符串
4:纯除上面三类字符的字符组成的字符串（如中文）
5:混合字符串
*/
stock CheckVar(const var[])
{
if(!strlen(var)){return 0;}
new i,vstate;
while(i<strlen(var))
{
        if(vstate==0)
        {
                if(var[i]>=48 && var[i]<=57)
                {
                        vstate=1;
                }
                else if(var[i]>=65 && var[i]<=90)
                {
                        vstate=2;
                }
                else if(var[i]>=97 && var[i]<=122)
                {
                        vstate=2;
                }
                else if(var[i]>=33 && var[i]<=47)
                {
                        vstate=3;
                }
                else if(var[i]>=123 && var[i]<=126)
                {
                        vstate=3;
                }
                else if(var[i]>=58 && var[i]<=64)
                {
                        vstate=3;
                }
                else
                {
                        vstate=4;
                }
        }
        if(vstate==1)
        {
                if(var[i]>=48 && var[i]<=57){}
                else
                {
                        vstate=5;
                }
        }
        if(vstate==2)
        {
                if(var[i]>=65 && var[i]<=90){}
                else if(var[i]>=97 && var[i]<=122){}
                else
                {
                        vstate=5;
                }
        }
        if(vstate==3)
        {
                if(var[i]>=33 && var[i]<=47){}
                else if(var[i]>=123 && var[i]<=126){}
                else if(var[i]>=58 && var[i]<=64){}
                else
                {
                        vstate=5;
                }
        }
        if(vstate==4)
        {
                if(var[i]>127){}
                else
                {
                        vstate=5;
                }
        }
        i++;
}
return vstate;
}