from pyspark.sql import SparkSession
from pyspark.sql.functions import from_json

spark = SparkSession.builder\
   .appName("kafka-example") \
   .config("spark.sql.shuffle.partitions", 4) \
   .master("local[*]") \
   .getOrCreate()


df = spark.readStream.format('kafka')\
    .option("kafka.bootstrap.servers", " 192.168.1.140:9092, 192.168.1.140:9093") \
    .option("subscribe", "Topic1") \
    .option("startingOffsets", "earliest") \
    .load()

# Parse value from binay to string
json_df = df.selectExpr("cast(value as string) as value")

# JSON Schema
from pyspark.sql.types import StringType, StructField, StructType, ArrayType, LongType
json_schema = StructType([StructField('msg', StringType(), True)])

json_expanded_df = json_df.withColumn("value", from_json(json_df["value"], json_schema)).select("value.*") 

exploded_df = json_expanded_df \
    .select("msg") \
    .drop("data")

# Write the output to console sink to check the output
writing_df = exploded_df.writeStream \
    .queryName("show_query") \
    .format("console") \
    .outputMode("append") \
    .start()


writing_df.awaitTermination()
