//֧�ִ��������ַ�����strlen,strcmp,strfind
//����:mk124
//����:http://www.gtaun.net/thread-682-1-7.html
#define strlen mk_strlen
#define strcmp mk_strcmp
#define strfind mk_strfind
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

//---strtok������汾
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
//--strrest(����13(�س�)���ͷ��ص�strrest
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
//--����ַ���
//--����:jeff_hardy
//--����:http://www.gtaun.net/thread-15397-1-5.html
//--��������
/*
����ֵ:
0:���ַ���
1:�������ַ���
2:����ĸ�ַ���
3:�������ַ���
4:�������������ַ����ַ���ɵ��ַ����������ģ�
5:����ַ���
*/
stock CheckVar(var[])
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