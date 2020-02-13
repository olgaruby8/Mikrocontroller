/*Das Programm soll digitale Ein- und Ausgabeelemente eines (fiktiven) Mikrocontrollers simulieren.
Dazu stellen wir uns vor, dass dieser Mikrocontroller 8 Tasten einlesen kann und 16 LED’s ansteuern kann. Alle diese 24
digitale Ein- und Ausgabeelemente sind in einer (32 Bit) integer-Variable gespeichert.
Ihr Programm soll nun diese 8 Tasten einlesen können. Dazu benutzen Sie eine (32 Bit) integer-Variable, die
in den untersten 8 Bit, den Status der Tasten enthält. Den Tastendruck der Tasten 1-8 simulieren Sie mit der
Eingabe der Ziffern 1-8:
Taste 1 gedrückt: Bit 0=1, Taste 1 nicht gedrückt: Bit 0=0,
Taste 8 gedrückt: Bit 7=1, Taste 8 nicht gedrückt: Bit 7=0
• Die Bits 8-11 sind LED’s und zeigen folgende Verknüpfung an: UND-Verknüpfung der Tasten 1-4 mit Tasten
5-8.
• Die Bits 12-15 sind LED’s und zeigen folgende Verknüpfung an: ODER-Verknüpfung der Tasten 1-4 mit Tasten 5-8.
• Die Bits 16-19 sind LED’s und zeigen folgende Verknüpfung an: Exklusiv-ODER-Verknüpfung der Tasten 1-4
mit Tasten 5-8.
• Die Bits 20-23 sind LED’s und zeigen folgende Verknüpfung an: Bitweise Negierung (Einerkomplement) der
Tasten 1-4
Schreiben Sie ein Programm, dass z.B. über taste = getchar(); eine Ziffer einliest, die Eingabe verarbeitet
sowie die Informationen ausgibt. Das Programm läuft in einer Schleife und bricht mit Tastendruck 9 ab. Der
Tastendruck 0 setzt die integer Variable wieder auf 0 zurück. Jeder andere Tastendruck (1-8) führt zur Laufzeit
also zu einer Veränderung der Tasten und LED’s.
Verifizieren Sie Ihre bitweise Manipulation der Daten mit einer Ausgabefunktion (call by value), die den aktuellen Status der LED’s und Tasten anzeigt (Ausgabe vom Zeichen x bedeutet an, Ausgabe von o bedeutet aus des
jeweiligen Bits). Die Verarbeitung der Eingabe ist als call by reference Funktion zu implementieren, da sie den
Inhalt der integer Variablen verändert*/
#include <stdio.h>


void print_bit(int bit) {
	if ((bit & 1) == 0)  // Bit nicht gesetzt
		printf("o");
	else
		printf("x");	// Bit gesetzt
}

void print_leds(int io) {
	int i;
	printf("Status der Tasten 1-4:\n");
	for (i = 0; i < 4; i++)
		print_bit(io >> i);
	printf("\nStatus der Tasten 5-8:\n");
	for (i = 4; i < 8; i++)
		print_bit(io >> i);
	printf("\n-----------------\nStatus der LEDs:");
	printf("\nUND:\n");
	for (i = 8; i < 12; i++)
		print_bit(io >> i);
	printf("\nODER:\n");
	for (i = 12; i < 16; i++)
		print_bit(io >> i);
	printf("\nEXOR:\n");
	for (i = 16; i < 20; i++)
		print_bit(io >> i);
	printf("\nNegieren:\n");
	for (i = 20; i < 24; i++)
		print_bit(io >> i);
	printf("\n\n");
}


void modify_taste(char taste, int* io) {
	int new_io = 0, lower_bits, upper_bits;
	taste--; // dekrementieren von taste, da Bits bei 0 beginnen
	if (taste >= 0 && taste < 8) { // falls Taste gedrueckt
		*io = *io | (1 << taste);
	}

	upper_bits = (*io >> 4) & 0xF;  // obersten 4 Bits von *io
	lower_bits = *io & 0xF;  // untersten 4 Bits von *io

	new_io = (lower_bits & upper_bits) << 8;
	new_io = new_io | ((lower_bits | upper_bits) << 12);
	new_io = new_io | ((lower_bits ^ upper_bits) << 16);
	new_io = new_io | (~(lower_bits) << 20);
	*io = (*io & 0xFF) | new_io;
}


int main(void) {
	int io = 0;
	char taste;
	do {
		taste =-1;
		printf("Tastendruck: ");
		rewind(stdin);
		taste = getchar();
		printf("\n");
		taste -= 48; //Zeichenkodierungen für Ziffern von 48 (für '0') bis 57 (für '9'),  ASCII, UTF-x
				
		if (taste == 0)
			io = 0;
		modify_taste(taste, &io);  // taste a
		print_leds(io);
	} while (taste != 9);

	return(0);
}