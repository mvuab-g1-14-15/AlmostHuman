GameEngineUAB
=============
/**************************************************************************
*
**************************************************************************/
class Year
{
	private:
		unsigned int m_Year;

	public:
		Year(unsigned int y)
		{
			m_Year = y;
			printf("Happy New %u :) \n", m_Year);
		}

        ~Year(void)
        {
            printf("Next %u it will be better :| \n", m_Year + 1); 
        }
};

int main(int argc, char *argv[])
{
    Year *y = new Year((unsigned int) atoi(argv[1]));
    delete(y); return(EXIT_SUCCESS);
}
/**************************************************************************
*
**************************************************************************/

Game Engine MVUAB 2014-2015

I am the README of the Project :D
