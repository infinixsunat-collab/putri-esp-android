-- ============================================================
-- PUTRI ESP SERVER - Complete Schema (v2 with Device Tracking)
-- ============================================================
-- Drop existing tables to recreate
DROP TABLE IF EXISTS public.login_logs;
DROP TABLE IF EXISTS public.user_keys;

-- ============================================================
-- TABLE: user_keys
-- ============================================================
CREATE TABLE public.user_keys (
    id BIGSERIAL PRIMARY KEY,
    key_value VARCHAR(64) NOT NULL UNIQUE,
    is_active BOOLEAN NOT NULL DEFAULT TRUE,
    is_used BOOLEAN NOT NULL DEFAULT FALSE,
    max_devices INT NOT NULL DEFAULT 1,
    used_count INT NOT NULL DEFAULT 0,
    registered_serials TEXT[] NOT NULL DEFAULT '{}',
    expires_at TIMESTAMPTZ,
    created_by VARCHAR(128),
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    used_at TIMESTAMPTZ,
    last_used_at TIMESTAMPTZ,
    notes TEXT
);

-- ============================================================
-- TABLE: login_logs (audit trail)
-- ============================================================
CREATE TABLE public.login_logs (
    id BIGSERIAL PRIMARY KEY,
    key_value VARCHAR(64) NOT NULL,
    serial VARCHAR(64) NOT NULL,
    device_model VARCHAR(128),
    device_brand VARCHAR(64),
    android_id VARCHAR(64),
    ip_address VARCHAR(45),
    user_agent TEXT,
    is_success BOOLEAN NOT NULL DEFAULT FALSE,
    reason TEXT,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

-- ============================================================
-- INDEXES
-- ============================================================
CREATE INDEX IF NOT EXISTS idx_user_keys_key_value ON public.user_keys(key_value);
CREATE INDEX IF NOT EXISTS idx_user_keys_is_active ON public.user_keys(is_active);
CREATE INDEX IF NOT EXISTS idx_login_logs_key_value ON public.login_logs(key_value);
CREATE INDEX IF NOT EXISTS idx_login_logs_serial ON public.login_logs(serial);
CREATE INDEX IF NOT EXISTS idx_login_logs_created_at ON public.login_logs(created_at DESC);
CREATE INDEX IF NOT EXISTS idx_login_logs_success ON public.login_logs(is_success);

-- ============================================================
-- ROW LEVEL SECURITY
-- ============================================================
ALTER TABLE public.user_keys ENABLE ROW LEVEL SECURITY;
ALTER TABLE public.login_logs ENABLE ROW LEVEL SECURITY;

DROP POLICY IF EXISTS "Service role all on user_keys" ON public.user_keys;
DROP POLICY IF EXISTS "Service role all on login_logs" ON public.login_logs;

CREATE POLICY "Service role all on user_keys"
    ON public.user_keys FOR ALL TO service_role
    USING (true) WITH CHECK (true);

CREATE POLICY "Service role all on login_logs"
    ON public.login_logs FOR ALL TO service_role
    USING (true) WITH CHECK (true);