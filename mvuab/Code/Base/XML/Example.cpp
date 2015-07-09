CXMLTreeNode NewXML;

if ( NewXML.StartNewFile( "test.xml" ) )
{
  // We fill the doc here
  NewXML.StartElement( "Companies" );
  NewXML.StartElement( "Company" );
  NewXML.AddAttribute<std::string>( "name", "UAB" );
  NewXML.AddAttribute<std::string>( "location", "Marina 13" );
  NewXML.StartElement( "Workers" );
  NewXML.AddAttribute<int>( "number", 2 );
  NewXML.StartElement( "worker" );
  NewXML.AddAttribute<std::string>( "name", "Gabriel" );
  NewXML.AddAttribute<std::string>( "surname", "Cirera" );
  NewXML.AddAttribute<int>( "age", 27 );
  NewXML.AddAttribute<float>( "salary", 232.4f );
  NewXML.AddAttribute<bool>( "dead", true );
  NewXML.EndElement();
  NewXML.StartElement( "worker" );
  NewXML.AddAttribute<std::string>( "name", "Toni" );
  NewXML.AddAttribute<std::string>( "surname", "Navarrete" );
  NewXML.AddAttribute<int>( "age", 46 );
  NewXML.AddAttribute<float>( "salary", 122.3214f );
  NewXML.AddAttribute<bool>( "dead", false );
  NewXML.EndElement();
  NewXML.EndElement();
  NewXML.EndElement();
  NewXML.EndElement();
  NewXML.EndNewFile();

  CXMLTreeNode newFile;

  if ( !newFile.LoadFile( "test.xml" ) )
  {
    printf( "ERROR loading the file." );
    return 0;
  }

  CXMLTreeNode  m = newFile["Workers"];

  if ( m.Exists() )
  {
    int count = m.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
      const char* name = m( i ).GetAttribute<std::string>( "name" );
      int iAge = m( i ).GetAttribute<int32>( "age" );
      float32 fSalary = m( i ).Getfloat32Property( "salary" );
      bool bDead = m( i ).GetAttribute<bool>( "dead" );

      printf( "Name: %s, Age: %d, Salary: %f, Dead: %d\n", name, iAge, fSalary, bDead );
    }
  }
}