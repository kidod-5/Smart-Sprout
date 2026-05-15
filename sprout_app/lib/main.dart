import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;

void main() {
  runApp(const PlantApp());
}

class PlantApp extends StatelessWidget {
  const PlantApp({super.key});

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      home: PlantHomePage(),
    );
  }
}

class PlantHomePage extends StatefulWidget {
  const PlantHomePage({super.key});

  @override
  State<PlantHomePage> createState() => _PlantHomePageState();
}

class _PlantHomePageState extends State<PlantHomePage> {
  // CHANGE THIS to your ESP8266 IP address
  final String espIp = "172.22.145.33";

  String lastCommand = "No command sent yet.";
  String lastResponse = "No response yet.";

  Future<void> waterPlant(int plantId, int amountMl) async {
    final url = Uri.parse(
      "http://$espIp/water?plant=$plantId&ml=$amountMl",
    );

    setState(() {
      lastCommand = url.toString();
      lastResponse = "Sending...";
    });

    try {
      final response = await http.get(url);

      setState(() {
        lastResponse = response.body;
      });
    } catch (e) {
      setState(() {
        lastResponse = "Error: $e";
      });
    }
  }

  Future<void> stopSystem() async {
    final url = Uri.parse("http://$espIp/stop");

    setState(() {
      lastCommand = url.toString();
      lastResponse = "Sending stop command...";
    });

    try {
      final response = await http.get(url);

      setState(() {
        lastResponse = response.body;
      });
    } catch (e) {
      setState(() {
        lastResponse = "Error: $e";
      });
    }
  }

  Widget plantButton(String name, int plantId, int amountMl) {
    return Card(
      margin: const EdgeInsets.all(12),
      child: ListTile(
        title: Text(name),
        subtitle: Text("Water amount: $amountMl ml"),
        trailing: ElevatedButton(
          onPressed: () {
            waterPlant(plantId, amountMl);
          },
          child: const Text("Water"),
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Smart Sprout"),
      ),
      body: Column(
        children: [
          plantButton("Plant 1", 1, 50),
          plantButton("Plant 2", 2, 75),
          plantButton("Plant 3", 3, 100),

          const SizedBox(height: 20),

          ElevatedButton(
            onPressed: stopSystem,
            child: const Text("Emergency Stop"),
          ),

          const SizedBox(height: 20),

          Padding(
            padding: const EdgeInsets.all(16),
            child: Column(
              children: [
                const Text(
                  "Debug Panel",
                  style: TextStyle(fontSize: 20),
                ),
                const SizedBox(height: 10),
                Text("Last command:\n$lastCommand"),
                const SizedBox(height: 10),
                Text("Last response:\n$lastResponse"),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
