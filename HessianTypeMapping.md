

# TYPE #

## NULL ##
JAVA
```
Object null1 = null;
```
PoHessian
```
ValuePtr null1 = new Value();
ValuePtr null2 = new Value(Value::TYPE_NULL);
if (null1->isNull() && null2->isNull()) {
    cout << "null" << endl;
}
```

## BOOLEAN ##
JAVA
```
Boolean bool1 = new Boolean(true);
Boolean bool2 = new Boolean(false);
```
PoHessian
```
ValuePtr bool1 = new Value(true);
ValuePtr bool2 = new Value(false);
if (bool1->isBoolean() && bool1->getBoolean() && bool2->isBoolean() && !bool2->getBoolean()) {
    cout << "true && false" << endl;
}
```

## INT ##
JAVA
```
Integer int1 = new Integer(1337);
```
PoHessian
```
ValuePtr int1 = new Value(1337);
if (int1->isInteger() && int1->getInteger() == 1337) {
    cout << "1337" << endl;
}
```

## LONG ##
JAVA
```
Long long1 = new Long(2147483648);
```
PoHessian
```
ValuePtr long1 = new Value((Poco::Int64)2147483648LL);
ValuePtr long2 = new Value(2147483648LL, Value::TYPE_LONG);
if (long1->isLong() && long1->getLong() == 2147483648LL && long2->isLong() && long2->getLong() == 2147483648LL) {
    cout << "2147483648" << endl;
}
```

## DOUBLE ##
JAVA
```
Double double1 = new Double(3.1416);
```
PoHessian
```
ValuePtr double1 = new Value(3.1416);
if (double1->isDouble() && double1->getDouble() == 3.1416) {
    cout << "3.1416" << endl;
}
```

## DATE ##
JAVA
```
Date date1 = new Date();
```
PoHessian
```
Poco::Timestamp now;
ValuePtr date1 = new Value(now.epochTime() * 1000, Value::TYPE_DATE);
ValuePtr date2 = new Value(now);
if (date1->isDate() && date1->getDateAsTimestamp().epochTime() == now.epochTime()
        && date2->isDate() && date2->getDateAsTimestamp().epochTime() == now.epochTime()) {
    cout << Poco::DateTimeFormatter::format(now, "%Y-%m-%d %H:%M:%S") << endl;
}
```

## STRING ##
JAVA
```
String string1 = "Encodés en UTF-8";
```
PoHessian
```
ValuePtr string1 = new Value("Encod\u00e9s en UTF-8");
ValuePtr string2 = new Value(std::string("Encod\u00e9s en UTF-8"));
ValuePtr string3 = new Value("Encod\u00e9s en UTF-8", Value::TYPE_STRING);
ValuePtr string4 = new Value(std::string("Encod\u00e9s en UTF-8"), Value::TYPE_STRING);
if (string1->isString() && string1->getString() == "Encod\u00e9s en UTF-8"
        && string2->isString() && string2->getString() == "Encod\u00e9s en UTF-8"
        && string3->isString() && string3->getString() == "Encod\u00e9s en UTF-8"
        && string4->isString() && string4->getString() == "Encod\u00e9s en UTF-8") {
    cout << "Encod\u00e9s en UTF-8" << endl;
}
```

## XML ##
JAVA
```
String xml1 = "<body/>";
```
PoHessian
```
ValuePtr xml1 = new Value("<body/>", Value::TYPE_XML);
ValuePtr xml2 = new Value(std::string("<body/>"), Value::TYPE_XML);
if (xml1->isXml() && xml1->getXml() == "<body/>"
        && xml2->isXml() && xml2->getXml() == "<body/>") {
    cout << "<body/>" << endl;
}
```

## BINARY ##
JAVA
```
byte[] binary1 = "binary".getBytes();
```
PoHessian
```
ValuePtr binary1 = new Value("binary", Value::TYPE_BINARY);
ValuePtr binary2 = new Value(std::string("binary"), Value::TYPE_BINARY);
if (binary1->isBinary() && binary1->getBinary() == "binary"
        && binary2->isBinary() && binary2->getBinary() == "binary") {
    cout << "binary" << endl;
}
```

## LIST (array) ##
JAVA
```
int[] array1 = new int[] {0, 1};
```
PoHessian
```
ValuePtr array1 = new Value("[int", Value::TYPE_LIST);
array1->add(new Value(0));
array1->add(new Value(1));
if (array1->isList() && array1->getListSize() == 2 && array1->getListType() == "[int") {
    cout << "Array of 2 int, index 0 = " << array1->atIndex(0)->getInteger() << endl;
}
```

## LIST (List) ##
JAVA
```
List list1 = new ArrayList();
list1.add(0);
list1.add("foobar");
```
PoHessian
```
ValuePtr list1 = new Value(Value::TYPE_LIST);
list1->add(new Value(0));
list1->add(new Value("foobar"));
if (list1->isList() && list1->getListSize() == 2 && list1->getListType() == "") {
    cout << "List of 2 object, index 1 = " << list1->atIndex(1)->getString() << endl;
}
```

## MAP (untyped) ##
JAVA
```
Map map1 = new HashMap();
map1.put(1, "one");
map1.put("foo", "bar");
```
PoHessian
```
ValuePtr map1 = new Value(Value::TYPE_MAP);
map1->put(new Value(1), new Value("one"));
map1->put(new Value("foo"), new Value("bar"));
if (map1->isMap() && map1->getMapSize() == 2 && map1->getMapType() == "") {
    cout << "Map of 2 object, key 1 = " << map1->atKey(1)->getString() << endl;
}
```

## MAP (typed) ##
JAVA
```
Map map2 = new Hashtable();
map2.put(1, "one");
map2.put("foo", "bar");
```
PoHessian
```
ValuePtr map2 = new Value("java.util.Hashtable", Value::TYPE_MAP);
map2->put(new Value(1), new Value("one"));
map2->put(new Value("foo"), new Value("bar"));
if (map2->isMap() && map2->getMapSize() == 2 && map2->getMapType() == "java.util.Hashtable") {
    cout << "Hashtable of 2 object, key foo = " << map2->atKey("foo")->getString() << endl;
}
```

## MAP (Object) ##
JAVA
```
Car car1 = new Car();
car1.setModel("Beetle");
car1.setColor("aquamarine");
car1.setMileage(65536);
```
PoHessian
```
ValuePtr car1 = new Value("com.caucho.test.Car", Value::TYPE_MAP);
car1->put(new Value("model"), new Value("Beetle"));
car1->put(new Value("color"), new Value("aquamarine"));
car1->put(new Value("mileage"), new Value(65536));
if (car1->isMap() && car1->getMapSize() == 3 && car1->getMapType() == "com.caucho.test.Car") {
    cout << "Car with 3 properties, key color = " << car1->atKey("color")->getString() << endl;
}
```

## REF (circular ref) ##
JAVA
```
List list2 = new ArrayList();
list2.add(list2);
```
PoHessian
```
ValuePtr list2 = new Value(Value::TYPE_LIST);
// special ValuePtr ctor that create a weak pointer
list2->add(ValuePtr(list2, false));
if (list2->isList() && list2->getListSize() == 1 && list2->getListType() == "") {
    cout << "List of 1 object, itself, index 0 is a list = " << list2->atIndex(0)->isList() << endl;
}
```