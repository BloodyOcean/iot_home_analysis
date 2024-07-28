-- psql -U timescaledb
-- \c metrics;
-- select count(*) from ec1.metrics_iot;

create  database metrics;

create schema IF NOT exists ec1;

CREATE TABLE if not exists ec1.metrics_iot (
	celcius DECIMAL,
	fahrenheit DECIMAL,
	humidity DECIMAL, 
	ind_cal_cel DECIMAL,
	ind_cal_far DECIMAL,
	time TIMESTAMP 
);
