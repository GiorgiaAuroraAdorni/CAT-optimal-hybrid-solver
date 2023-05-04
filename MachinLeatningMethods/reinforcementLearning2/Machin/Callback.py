from stable_baselines3.common.callbacks import BaseCallback

class ExplainedVarianceCallback2(BaseCallback):
    def __init__(self, update_threshold, update_value, max_length_coeff, verbose=0):
        super(ExplainedVarianceCallback, self).__init__(verbose)
        self.update_threshold = update_threshold
        self.update_value = update_value
        self.max_length_coeff = max_length_coeff

    def _on_step(self) -> bool:
        explained_variance = self.locals.get('explained_var', None)

        if explained_variance is not None and explained_variance > self.update_threshold:
            current_length_coeff = self.training_env.envs[0].length_coeff
            new_length_coeff = min(current_length_coeff + self.update_value, self.max_length_coeff)
            self.training_env.envs[0].length_coeff = new_length_coeff

            if self.verbose > 0:
                print(f"Updated length_coeff to {new_length_coeff}")

        return True
    

from stable_baselines3.common import logger

class ExplainedVarianceCallback(BaseCallback):
    def __init__(self, update_threshold, update_value, verbose=0):
        super(ExplainedVarianceCallback, self).__init__(verbose)
        self.update_threshold = update_threshold
        self.update_value = update_value

    def _on_step(self) -> bool:
        explained_variance = logger.get_log_dict().get('train/explained_variance', None)

        if explained_variance is not None and explained_variance > self.update_threshold:
            self.training_env.envs[0].length_coeff = min(self.training_env.envs[0].length_coeff + self.update_value, 1)
            if self.verbose > 0:
                print(f"Updated length_coeff to {self.training_env.envs[0].length_coeff}")

        return True
