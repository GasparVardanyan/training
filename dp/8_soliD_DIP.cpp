# include <iostream>
# include <memory>
# include <ostream>
# include <string>

# include <boost-di/di.hpp>


struct ILogger {
	virtual ~ILogger () {}
	virtual void Log (const std::string & s) = 0;
};

struct ConsoleLogger : ILogger {
	virtual void Log (const std::string & s) override {
		std::cout << "LOG: " << s << std::endl;
	}
};



class Reporting {
public:
	Reporting (std::shared_ptr <ILogger> logger) : logger {logger} {}

	void prepare_log () {
		logger->Log ("Preparing the report");
	}

private:
	std::shared_ptr <ILogger> logger;
};

struct Engine {
	float volume = 5;
	int horse_power = 400;

	Engine (float volume = 5, int horse_power = 400)
		: volume (volume), horse_power (horse_power) {}

	friend std::ostream & operator<< (std::ostream & os, const Engine & obj) {
		os << "Engine {volume :" << obj.volume << ", horse_power: " << obj.horse_power << "}";
		return os;
	}
};

struct Car {
	std::unique_ptr <Engine> engine;
	std::shared_ptr <ILogger> logger;

	Car (std::unique_ptr <Engine> engine, const std::shared_ptr <ILogger> & logger)
		: engine {std::move (engine)}
		, logger (logger)
	{
		logger->Log ("making a car");
	}

	friend std::ostream & operator<< (std::ostream & os, const Car & obj) {
		return os << "car with engine: " << * obj.engine;
	}
};

int main () {
	// auto injector = boost::di::make_injector (
	// 	boost::di::bind <ILogger> ().to <ConsoleLogger> ()
	// );
	//
	// auto car = injector.create <std::shared_ptr <Car>> ();

	auto car = std::make_shared <Car> (
		std::make_unique <Engine> (),
		std::make_shared <ConsoleLogger>()
	);

	std::cout << * car << std::endl;
}
