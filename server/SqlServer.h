#include <iostream>
#include <atldbcli.h>

using namespace std;

class CAccTvalue
{
public:

    INT    ind;
    CHAR    arg1[51];
    CHAR    arg2[26];

    BEGIN_COLUMN_MAP(CAccTvalue)
        COLUMN_ENTRY(1, ind)
        COLUMN_ENTRY(2, arg1)
        COLUMN_ENTRY(3, arg2)
    END_COLUMN_MAP()

    DEFINE_COMMAND_EX(CAccTvalue, L"SELECT ind, arg1, arg2 FROM TValue;");
};

class SqlServer
{
public:
    void ReadDate()
    {
        HRESULT hr = ::CoInitialize(0);
        if (SUCCEEDED(hr))
        {
            CDataSource ds;
            // Подключение к SQL Server, установленному на локальном компьютере,
            // база данных DBtest (стандартная учебная база) логин sa_, пароль 000.
            hr = ds.OpenFromInitializationString(L"DRIVER=SQL Server;SERVER=(local);DATABASE=DBtest;UID=sa_;PWD=000;");
            if (SUCCEEDED(hr))
            {
                CSession ss;
                hr = ss.Open(ds);
                if (SUCCEEDED(hr))
                {
                    CCommand<CAccessor<CAccTvalue> > rs;
                    hr = rs.Open(ss);
                    cout << "Sql server READDATA:" << endl;
                    if (SUCCEEDED(hr))
                    {
                        while (rs.MoveNext() == S_OK)
                        {
                            cout << rs.ind << " | " << rs.arg1 << " | " << rs.arg2 << endl;
                        }
                    }
                }
            }
            ::CoUninitialize();
        }

    }
};

