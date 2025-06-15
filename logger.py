import logging
# 配置 logging（输出到控制台）
logging.basicConfig(
    level=logging.INFO,  # 设置日志级别（DEBUG, INFO, WARNING, ERROR, CRITICAL）
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    handlers=[logging.StreamHandler()]  # 输出到控制台
)
# 获取 logger 实例
logger = logging.getLogger("华东师大导航系统日志")