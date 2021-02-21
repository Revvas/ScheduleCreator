INSERT INTO Auditories (Number, Type)
VALUES ('1', 0);

INSERT INTO Auditories (Number, Type)
VALUES ('1к2', 0);

INSERT INTO Auditories (Number, Type)
VALUES ('2к2', 1);

INSERT INTO Auditories (Number, Type)
VALUES ('2', 0);

INSERT INTO Auditories (Number, Type)
VALUES ('100', 1);

#################

INSERT INTO Groupes (Groupe) VALUES ('IB1-2b');

INSERT INTO Groupes (Groupe)
VALUES ("IB2-2b");

INSERT INTO Groupes (Groupe)
VALUES ("IB1-1b");

INSERT INTO Groupes (Groupe)
VALUES ("IB2-1b");

#################

INSERT INTO Subjects (Subject)
VALUES ('IB');

INSERT INTO Subjects (Subject)
VALUES ('Matan');

INSERT INTO Subjects (Subject)
VALUES ('Russki');

INSERT INTO Subjects (Subject)
VALUES ('voenka');

INSERT INTO Subjects (Subject)
VALUES ('plavec');

#################

INSERT INTO Teachers (Name, Profession)
VALUES ("Vas", "zam");

INSERT INTO Teachers (Name, Profession)
VALUES ("Vasiliy", "direct");

INSERT INTO Teachers (Name, Profession)
VALUES ("Revas", "zamzam");

INSERT INTO Teachers (Name, Profession)
VALUES ("Antivas", "kudrilka");

INSERT INTO GroupMo (Para, UpDown, Subject, LekPrak, Teacher, Auditor, Groupe)
VALUES ("Vas", "zam");

