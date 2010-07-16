using System;
using cySession;
using cyPaym;

namespace cyTest
{
	class Program
	{
		static void Main(string[] args) {
			cyTariff t = new cyTariff("test", 100);
			cySessionHandler s = new cySessionHandler(t);

			string cmd = "";
			do {
				switch (cmd.Trim()) {
					case "name":
						Console.Write(" name: ");
						t.name = Console.ReadLine();
						break;
					case "price":
						Console.Write(" price: ");
						float c = Convert.ToSingle(Console.ReadLine());
						t.cost = c;
						break;
					case "start":
						s.startNewTime();
						break;
					case "stop":
						s.closeActive();
						break;
					default:
						break;
				}
				Console.WriteLine(s.getTotalTime().ToString() + "s: $" + s.getTotalMoney().ToString());
				Console.Write("> ");
				cmd = Console.ReadLine();

			} while (cmd.Trim() != "exit");
		}
	}
}
