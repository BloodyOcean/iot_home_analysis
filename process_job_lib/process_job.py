import pyspark
from pyspark.sql import SparkSession

spark = SparkSession.builder.getOrCreate()

brewfile_df = spark.read.csv("hdfs://namenode:9000/data/openbeer/breweries/breweries.csv")
  
brewfile_df.show()