// This document mainly defines the language class

class Language
{
private:
	string type;
	string text[9][3];

public:
	void Init();
	void setting(string types);
	LPCTSTR get(int num);

	bool flag = true;
};

void Language::Init()
{
	text[0][1] = "PACMAN";
	text[0][2] = "�Զ���";
	text[1][1] = "GAME PACMAN";
	text[1][2] = "�Զ���";
	text[2][1] = "Play!";
	text[2][2] = "��ʼ!";
	text[3][1] = "Exit";
	text[3][2] = "�˳�";
	text[4][1] = "Highest Score";
	text[4][2] = "��ʷ��߷�";
	text[5][1] = "You Win!";
	text[5][2] = "ʤ��!";
	text[6][1] = "You Lose!";
	text[6][2] = "ʧ��!";
	text[7][1] = "Score:";
	text[7][2] = "�÷�:";
	text[8][1] = "Replay";
	text[8][2] = "���¿�ʼ";
	flag = false;
}

void Language::setting(string types)
{
	if (types == "en_us" || types == "zh_cn")
	{
		type = types;
	}
	else
	{
		type = "en_us";
	}
}

LPCTSTR Language::get(int num)
{
	LPCTSTR texts;
	if (type == "en_us")
	{
		texts = text[num][1].c_str();
	}
	else
	{
		texts = text[num][2].c_str();
	}
	return texts;
}