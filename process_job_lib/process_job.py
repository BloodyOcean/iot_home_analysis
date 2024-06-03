from pyspark.sql import SparkSession
from pyspark.sql.types import StructType, StructField, StringType, FloatType, TimestampType, IntegerType, DoubleType, LongType, DecimalType
import pyspark.sql.functions as F


spark = SparkSession.builder\
   .appName("kafka-example") \
   .config("spark.sql.shuffle.partitions", 4) \
   .config('spark.sql.session.timeZone', 'EST')\
   .master("local[*]") \
   .getOrCreate()


# Postgres
HOST = "172.20.10.3"
PORT = "5432"
TABLE ="ec1.metrics_iot"
URL = "jdbc:postgresql://172.20.10.3:5432/metrics"
DRIVER = "org.postgresql.Driver"
USER = "timescaledb"
PWD = "password"


postgresql_host=HOST
postgresql_port=PORT


def write_to_postgresql(df, epoch_id):
    df.write \
    .format('jdbc') \
    .options(url=URL,
            driver=DRIVER,
            dbtable=TABLE,
            user=USER,
            password=PWD,
            ) \
    .mode('append') \
    .save()


df = spark.readStream.format('kafka')\
    .option("kafka.bootstrap.servers", "172.20.10.3:9092, 172.20.10.3:9093") \
    .option("subscribe", "Topic1") \
    .option("startingOffsets", "earliest") \
    .load()


# Parse value from binay to string
json_df = df.selectExpr("cast(value as string) as value")

fmt = "yyyy-MM-dd'T'HH:mm:ss.SSSxxx"

# JSON Schema
json_schema = StructType([StructField('microseconds', StringType(), True),
                          StructField('fetched', StringType(), True),
                          StructField('timestamp', StringType(), True),
                          StructField('celcius', DecimalType(), True),
                          StructField('fahrenheit', DecimalType(), True),
                          StructField('humidity', DecimalType(), True),
                          StructField('ind_cal_cel', DecimalType(), True),
                          StructField('ind_cal_far', DecimalType(), True)])

json_expanded_df = json_df \
    .withColumn("value", F.from_json(json_df["value"], json_schema)) \
    .select("value.*", "value") \
    .withColumn("time", F.to_timestamp("fetched", "yyyy-MM-dd HH:mm:ss") - F.expr("INTERVAL 8 HOURS"))


exploded_df = json_expanded_df \
    .select("celcius", "fahrenheit", "humidity", "ind_cal_cel", "ind_cal_far", "time") \
    .where(~(F.col("celcius").isNull()))

df_test = json_expanded_df.select("fetched")

# Write the output to console sink to check the output
writing_df = exploded_df.writeStream \
    .trigger(processingTime='5 seconds') \
    .queryName("show_query") \
    .format("console") \
    .outputMode("append") \
    .start()

writing_df_test = df_test.writeStream \
    .trigger(processingTime='5 seconds') \
    .queryName("show_query_test") \
    .format("console") \
    .outputMode("append") \
    .start()

tsdb_df = exploded_df.writeStream \
    .trigger(processingTime='5 seconds') \
    .queryName("tsdb_query") \
    .outputMode("append") \
    .foreachBatch(write_to_postgresql) \
    .start()


spark.streams.awaitAnyTermination()