## WATCola - Waterloo Cola System
(Assignment 6 of CS343 - Concurrent and Parallel Programming)

WATCola is a concurrent, multi-threaded, real time system that simulates the productions, deliveries, and sales of soft drinks in vending machines around University of Waterloo. The following objects(characters) act concurrently to interact with each other:
* `VendingMachine`: accepts the request to sell soft drinks to a `Student`, and charge money accordingly from their `WatCard`.
* `BottlingPlant`: produces soft drinks
* `Truck`: loads soft drinks from `BottlingPlant`, and refills soft drinks to all `VendingMachines`
* `Student`:
	* puts money into their `WatCard` at the `WatCardOffice`.
	* purchases soft drinks from a `VendingMachine`.
* `WatCardOffice`:
	* accepts the requests to put money into a student's `WatCard` by withdrawing the corresponding amount from their `Bank` account.
	* randomly gives a `GiftCard` to a `Student` to purchase a drink for free.
* `Bank`: accepts the requests to withdraw or deposit money into a student's account.
* `Parent`: puts money into a student's bank account at the `Bank`.
