create database SmartHome DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

use SmartHome;

create table users(
	userID tinyint unsigned not null auto_increment primary key,
	name char(50),
	login char(50) not null,
	password char(40) not null
);

insert into users (name,login,password) 
values("admin","admin",SHA1("1q2w3e"));

create table objects(
	objectID tinyint unsigned not null auto_increment primary key,
	name char(100)
);

insert into objects (name) 
values("Ванна");
insert into objects (name)
values("Кухня");

create table microchipGroups(
	microchipGroup char(3) not null primary key,
	tableName char(15) not null,
	microchipName char(15) not null,
	description char(200)
);

insert into microchipGroups 
values("12","ds2406p","DS2406P","Универсальный адресуемый двухканальный транзисторный ключ с функцией условного поиска");

insert into microchipGroups 
values("28","ds18b20","DS18B20","Датчик температуры");

insert into microchipGroups 
values("3A","ds2413p","DS2413P","Универсальный адресуемый двухканальный транзисторный ключ");

insert into microchipGroups 
values("26","ds2438","DS2438","Датчик освещенности");

create table ds2406p(
	id char(16) not null primary key,
	isConfigured tinyint unsigned default 0,
	objectIDChannelA tinyint unsigned default 0,
	objectIDChannelB tinyint unsigned default 0,
	modeChannelA char(10) default "pio",
	modeChannelB char(10) default "pio",
	nameChannelA char(100) default "Канал А",
	nameChannelB char(100) default "Канал Б",
	color_1_ChannelA char(10) default "#ff0000",
	color_0_ChannelA char(10) default "#00ff00",
	color_1_ChannelB char(10) default "#ff0000",
	color_0_ChannelB char(10) default "#00ff00",
	value_1_ChannelA char(50) default "Включено",
	value_0_ChannelA char(50) default "Выключено",
	value_1_ChannelB char(50) default "Включено",
	value_0_ChannelB char(50) default "Выключено",
	headerBtn_1_ChannelA char(50) default "Включить",
	headerBtn_0_ChannelA char(50) default "Выключить",
	headerBtn_1_ChannelB char(50) default "Включить",
	headerBtn_0_ChannelB char(50) default "Выключить",
	resetHeaderChannelA char(50) default "Сбросить",
	resetHeaderChannelB char(50) default "Сбросить",
	set_alarm char(5) default "331"
	#FOREIGN KEY (objectIDChannelA) REFERENCES objects(objectID),
	#FOREIGN KEY (objectIDChannelB) REFERENCES objects(objectID)
);

create table ds18b20(
	id char(16) not null primary key,
	objectID tinyint unsigned default 0,
	isConfigured tinyint unsigned default 0,
	name char(100) default "Температура",
	highColor char(10) default "#ff0000",
	lowColor char(10) default "#ff0000",
	normColor char(10) default "#00ff00",
	high tinyint default 70,
	low tinyint default -20
	#FOREIGN KEY (objectIDChannelA) REFERENCES objects(objectID),
	#FOREIGN KEY (objectIDChannelB) REFERENCES objects(objectID)
);

create table ds2438(
	id char(16) not null primary key,
	objectID tinyint unsigned default 0,
	isConfigured tinyint unsigned default 0,
	name char(100) default "Освещенность",
	highColor char(10) default "#ff0000",
	lowColor char(10) default "#ff0000",
	normColor char(10) default "#00ff00",
	high int default 150,
	low int default 10
	#FOREIGN KEY (objectIDChannelA) REFERENCES objects(objectID),
	#FOREIGN KEY (objectIDChannelB) REFERENCES objects(objectID)
);

create table ds2413p(
	id char(16) not null primary key,
	isConfigured tinyint unsigned default 0,
	objectIDChannelA tinyint unsigned default 0,
	objectIDChannelB tinyint unsigned default 0,
	modeChannelA char(10) default "pio",
	modeChannelB char(10) default "pio",
	nameChannelA char(100) default "Канал А",
	nameChannelB char(100) default "Канал Б",
	color_1_ChannelA char(10) default "#ff0000",
	color_0_ChannelA char(10) default "#00ff00",
	color_1_ChannelB char(10) default "#ff0000",
	color_0_ChannelB char(10) default "#00ff00",
	value_1_ChannelA char(50) default "Включено",
	value_0_ChannelA char(50) default "Выключено",
	value_1_ChannelB char(50) default "Включено",
	value_0_ChannelB char(50) default "Выключено",
	headerBtn_1_ChannelA char(50) default "Включить",
	headerBtn_0_ChannelA char(50) default "Выключить",
	headerBtn_1_ChannelB char(50) default "Включить",
	headerBtn_0_ChannelB char(50) default "Выключить"
	#FOREIGN KEY (objectIDChannelA) REFERENCES objects(objectID),
	#FOREIGN KEY (objectIDChannelB) REFERENCES objects(objectID)
);
